using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;
using System.Threading;


namespace cshapetest
{
    struct Msg
    {
        internal byte[] bytes;
        internal int length;
        internal DateTime getTime; 
    }
    public partial class Form1 : Form
    {
        static Queue<Msg> msg_queue;
        static Socket socket;
        static AutoResetEvent conditional_variable;
        static Object locker;
        static volatile bool quit_flag = false;
        static IPEndPoint ep;

        public int[,,] SCAData = new int[10000,10,10];    //40为一秒轮次，可存储25秒数据
        string[,] DataGetTime = new string[10000,10];       

        private int RunFlag = 0;                   //是否运行标志
        public int udpCnt = 1;
        public int getInfoTimes = 0;               //波峰检测标志
        Socket udpRecv = null;
        Socket udpSend = null;

        private int firstProcessFlag = 0;

        public int CycleCount = 0;
        public int chnldx;
        public string getTime = "";
        public string currenTime = "";
        public int count = 0;
        public int gapsum = 0;

        DateTime lastBeginRecive;
        DateTime endRecive;

        public int DataLen = 0;

        public int sendflag = 0;  //是否发送udp数据
         

        public Form1()
        {
            InitializeComponent();
        }


        private void Form1_Load(object sender, EventArgs e)
        {
            

            msg_queue = new Queue<Msg>();
            conditional_variable = new AutoResetEvent(false);
            locker = new object();
          
            udpSend = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            IPEndPoint sendpoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 1130);
            //udpSend.Bind(sendpoint);
            udpSend.Connect(sendpoint);
        }


        private void start_Click(object sender, EventArgs e)
        {
            udpRecv = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            IPEndPoint endpoint = new IPEndPoint(IPAddress.Parse("172.28.110.110"), 27001);          //172.28.110.110   27001
            udpRecv.Bind(endpoint);

            Thread listen_thread = new Thread(RecvMessage);
            Thread process_thread1 = new Thread(ProcessMessage);
            Thread process_therad2 = new Thread(ProcessMessage);
            Thread process_thread3 = new Thread(ProcessMessage);
            Thread process_therad4 = new Thread(ProcessMessage);

            listen_thread.Start();

            process_thread1.Start();
            process_therad2.Start();
            process_thread3.Start();
            process_therad4.Start();

        }

        private void RecvMessage()
        {
            while (!quit_flag)
            {
                byte[] buf = new byte[10000];
                if (buf == null)
                    throw new Exception();
                int read = 0;
                try
                {
                    read = socket.Receive(buf);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                    if (quit_flag)
                        return;
                }
                Msg msg;
                msg.bytes = buf;
                msg.length = read;
                msg.getTime  = DateTime.Now;
                lock (locker)
                {
                    msg_queue.Enqueue(msg);
                }
                conditional_variable.Set();
            }
        }

        private void ProcessMessage()
        {
            while (!quit_flag)
            {
                while (msg_queue.Count == 0)
                {
                    conditional_variable.WaitOne();
                    if (quit_flag)
                        return;
                }
                Msg msg;
                lock (locker)
                {
                    msg = msg_queue.Dequeue();
                }
                
                string s = bytetohexstr(msg.bytes, msg.length);
                string rawdata = s.Replace(" ", "");     //视情况可删减
                dataProcess dp = new dataProcess();

                label1.Text = dp.processway.ToString();

                int index = 0;
                CycleCount ++;
                while (index < rawdata.Length - 6)
                {
                    int SectionLen = 0;
                    int SectionType = 0;

                    SectionLen = dp.GetN(rawdata, ref index, 2);
                    SectionType = dp.GetN(rawdata, ref index, 1);

                    switch (SectionType)
                    {
                        case 1:
                            chnldx = dp.ChannelMetaSection(ref index, SectionLen, rawdata);
                            break;
                        case 2:
                            dp.SpectralMetaSection(ref index, SectionLen, rawdata);
                            break;
                        case 3:
                            dp.SpectrumSection(ref index, SectionLen, rawdata);
                            break;
                        case 4:
                            dp.DiagnosticDataSection(ref index, SectionLen, rawdata);
                            break;
                        case 5:
                            dp.SCADataSection(ref index, SectionLen, rawdata);
                            break;
                        default:
                            Console.WriteLine("Invalid Data!");
                            break;
                    }       
                 }

                if (chnldx >= 0 && chnldx < 11)
                {
                    if(sendflag == 0)
                    {
                        int j = 0;
                        for (int i = 0; i < 3; i++)
                        {
                            SCAData[CycleCount / 10, chnldx - 1, j++] = dp.SCAStartBin[i];
                            SCAData[CycleCount / 10, chnldx - 1, j++] = dp.SCALength[i];
                            SCAData[CycleCount / 10, chnldx - 1, j++] = dp.SCACount[i];
                        }
                        SCAData[CycleCount / 10, chnldx - 1, 9] = chnldx;
                        DataGetTime[CycleCount / 10, chnldx - 1] = msg.getTime.ToString();
                    }
                    else
                    {
                        if(dp.SCACount[0] > 15)
                        {
                            byte[] gfData = new byte[10];
                            TimeSpan timeStamp = msg.getTime - new DateTime(1970, 1, 1, 0, 0, 0, 0);
                            long temp = (Int64)timeStamp.TotalMilliseconds;
                            for (int i = 0; i < 8; i++)
                            {
                                gfData[i] = (byte)(temp >> (i * 8) & 0xff);
                            }
                            byte[] c = new byte[2];
                            c = BitConverter.GetBytes(chnldx);
                            gfData[8] = c[0];
                            gfData[9] = c[1];
                            udpSend.Send(gfData);

                        }                       

                    }
                   
                }
                else
                {
                    MessageBox.Show("channel index error!");
                }
            }
        }

        public string bytetohexstr(byte[] bytes, int length)     //字节转化为二进制
        {
            string returnstr = "";
            if (bytes != null)
            {
                for (int i = 0; i < length; i++)
                {
                    returnstr += bytes[i].ToString("X2");
                }
            }
            return returnstr;
        }

        public void write(int[,,] arr)             //保存txt文本
        {
            string temppath = @"D:\422test\data";
            
            
            for (int k = 0; k < CycleCount/10; k ++)
            {               
                string txtname = temppath + k;
                txtname = txtname + ".txt";     
                FileStream fs = new FileStream(txtname, FileMode.Create);
                StreamWriter sw = new StreamWriter(fs);

                 for (int i = 0; i < 10; ++i)
                 {
                     sw.WriteLine(i);
                     sw.WriteLine(DataGetTime[k,i]);
                     for (int j = 0; j < 10; ++j)
                     {
                         sw.Write(arr[k , i , j] + " ");
                     }
                     sw.WriteLine();
                 }                                           
                //清空缓冲区
                sw.Flush();
                //关闭流
                sw.Close();
                fs.Close();
            }
            MessageBox.Show("write finish!");


        }


       
        private void Stop_Click(object sender, EventArgs e)
        {
            quit_flag = true;
            write(SCAData);
            socket.Close();
            conditional_variable.Set();
            conditional_variable.Set();              
        }

        private void testUdp_Click(object sender, EventArgs e)
        {
            

            byte[] gfData = new byte[10];
            DateTime testtime = new DateTime(2021, 8, 25, 0, 0, 0, 0);
            TimeSpan timeStamp = testtime - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            long temp = (Int64)timeStamp.TotalMilliseconds;
            for (int i = 0; i < 8; i++)
            {
                gfData[i] = (byte)(temp >> (i * 8) & 0xff);
            }
            byte[] c = new byte[2];
            int tempindex = 2;
            c = BitConverter.GetBytes(tempindex);
            gfData[8] = c[0];
            gfData[9] = c[1];
            udpSend.Send(gfData);

        }
    }

    class dataProcess
    {

        public int[] SData = new int[2048];
        public int processway = 1;

        //数据一
        public int Arridx, Chnidx, IfcVerMajor, IfcVerMinor;
        public int DppAvailable, SpectralDataAvailabel, SnapshotStatisticDataValid, MxPowerSupplyState, MxPowerSupplyStable, SddTemperatureReady;
        public int ChannelState, CntCommunicationReset, CntUdpPacket, CntUdpSendErr, CntMissedPacket, CntSyncErr, CntSnapshotStatInvalid;
        public float DppTemperature, SddTemperature;

        //数据二
        public int SpectCtr, Events, BytesPerBin, NumberOfBins, StartBin;
        public float DppTransferTime, Realtime, Lifetime, InputCountRate, OutputCountRate, DeadtimeRatio;

        //数据三
        public int TemperatureWarning, TemperatureFault, MxPowerUndervoltageLockout, MixedModeDetected;
        public float MxCpuTemperature, MxCpuUpperSurfaceTemperature, MxCpuUnderSurfaceTemperature, MxCpuLoadCore1, MxCpuLoadCore2, MxPowerTemperature;

        //数据五
        //public int SCAStartBin ,SCALength ,SCACount;
        public int[] SCAStartBin = new int[4];
        public int[] SCALength = new int[4];
        public int[] SCACount = new int[4];


        public int GetN(string data, ref int index, int n)
        {
            int ret = 0; int j = 0;
            for (int i = index + 2 * n - 1; i >= index; i--)
            {
                int temp = data[i];
                if (temp > 47 && temp < 58)
                {
                    temp -= 48;
                }
                else if (temp > 64 && temp < 71)
                {
                    temp -= 55;
                }
                double powAns = Math.Pow(16, j);
                ret = ret + temp * (int)powAns;
                j++;
            }
            index += 2 * n;
            return ret;
        }

        public float strToFloat(string data)
        {
            uint num = uint.Parse(data, System.Globalization.NumberStyles.AllowHexSpecifier);
            byte[] floatVals = BitConverter.GetBytes(num);
            float f = BitConverter.ToSingle(floatVals, 0);
            return f;
        }

        public int ChannelMetaSection(ref int index, int SectionLen, string rawdata)
        {
            if(processway == 1)
            {
                index += 2;
                Chnidx = GetN(rawdata, ref index, 1);
                index += 50;
            }
            else
            {
                Arridx = GetN(rawdata, ref index, 1);
                Chnidx = GetN(rawdata, ref index, 1);
                IfcVerMajor = GetN(rawdata, ref index, 1);
                IfcVerMinor = GetN(rawdata, ref index, 1);

                string temp;
                int convertAns;
                temp = rawdata.Substring(index, 4);
                string Ans = Convert.ToString(Convert.ToInt32(temp, 16), 2).PadLeft(16, '0');

                DppAvailable = Ans[15] - 48;
                SpectralDataAvailabel = Ans[14] - 48;
                SnapshotStatisticDataValid = Ans[13] - 48;
                MxPowerSupplyState = Ans[12] - 48;
                MxPowerSupplyStable = Ans[11] - 48;
                SddTemperatureReady = Ans[10] - 48;

                index += 4;

                ChannelState = GetN(rawdata, ref index, 1);
                CntCommunicationReset = GetN(rawdata, ref index, 2);
                CntUdpPacket = GetN(rawdata, ref index, 2);
                CntUdpSendErr = GetN(rawdata, ref index, 2);
                CntMissedPacket = GetN(rawdata, ref index, 2);
                CntSyncErr = GetN(rawdata, ref index, 2);
                CntSnapshotStatInvalid = GetN(rawdata, ref index, 2);

                temp = rawdata.Substring(index, 8);
                DppTemperature = strToFloat(temp);
                index += 8;

                temp = rawdata.Substring(index, 8);
                SddTemperature = strToFloat(temp);
                index += 8;
            }
            

            return Chnidx;
          
        }

        public void SCADataSection(ref int index, int SectionLen, string rawdata)
        {
            int tempstartbin, templength, tempcount;
            for(int i = 0; i < 16; i++)
            {
                tempstartbin = GetN(rawdata, ref index, 2);
                templength = GetN(rawdata, ref index, 2);
                tempcount = GetN(rawdata, ref index, 4);
                /*if(i == Chnidx - 1)
                {
                    SCAStartBin = tempstartbin;
                    SCALength = templength;
                    SCACount = tempcount;
                }*/
                if( i >= 0 && i < 3)
                {
                    SCAStartBin[i] = tempstartbin;
                    SCALength[i] = templength;
                    SCACount[i] = tempcount;
                }
                
            }
           
        }

        public void SpectralMetaSection(ref int index, int SectionLen, string rawdata)
        {
            if (processway == 1)
            {
                index += 70;
            }
            else
            {
                SpectCtr = GetN(rawdata, ref index, 2);

                string temp;
                temp = rawdata.Substring(index, 8);
                DppTransferTime = strToFloat(temp);
                index += 8;

                temp = rawdata.Substring(index, 8);
                Realtime = strToFloat(temp);
                index += 8;

                temp = rawdata.Substring(index, 8);
                Lifetime = strToFloat(temp);
                index += 8;

                Events = GetN(rawdata, ref index, 4);

                temp = rawdata.Substring(index, 8);
                InputCountRate = strToFloat(temp);
                index += 8;

                temp = rawdata.Substring(index, 8);
                OutputCountRate = strToFloat(temp);
                index += 8;

                temp = rawdata.Substring(index, 8);
                DeadtimeRatio = strToFloat(temp);
                index += 8;

                BytesPerBin = GetN(rawdata, ref index, 1);
                NumberOfBins = GetN(rawdata, ref index, 2);
                StartBin = GetN(rawdata, ref index, 2);
            }
            
        }

        public void SpectrumSection(ref int index, int SectionLen, string rawdata)
        {
            if (processway == 1)
            {
                index += (SectionLen - 1) * 2;
            }
            else
            {
                int start = index;
                for (int i = 0; index < start + (SectionLen - 1) * 2; i++)
                {
                    SData[i] = GetN(rawdata, ref index, 2);
                }
            }
            

        }

        public void DiagnosticDataSection(ref int index, int SectionLen, string rawdata)
        {
            if (processway == 1)
            {
                index += 52;
            }
            else
            {
                string temp;
                temp = rawdata.Substring(index, 4);
                string Ans = Convert.ToString(Convert.ToInt32(temp, 16), 2).PadLeft(16, '0');
                TemperatureWarning = Ans[15] - 48;
                TemperatureFault = Ans[14] - 48;
                MxPowerUndervoltageLockout = Ans[13] - 48;
                MixedModeDetected = Ans[12] - 48;
                index += 4;

                temp = rawdata.Substring(index, 8);
                MxCpuTemperature = strToFloat(temp);
                index += 8;

                temp = rawdata.Substring(index, 8);
                MxCpuUpperSurfaceTemperature = strToFloat(temp);
                index += 8;

                temp = rawdata.Substring(index, 8);
                MxCpuUnderSurfaceTemperature = strToFloat(temp);
                index += 8;

                temp = rawdata.Substring(index, 8);
                MxCpuLoadCore1 = strToFloat(temp);
                index += 8;

                temp = rawdata.Substring(index, 8);
                MxCpuLoadCore2 = strToFloat(temp);
                index += 8;

                temp = rawdata.Substring(index, 8);
                MxPowerTemperature = strToFloat(temp);
                index += 8;

            }
        }
    }
}
