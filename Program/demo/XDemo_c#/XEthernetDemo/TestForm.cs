#define _TEST
//#define _OLD
#define _IO
//#define _DEEP
#define _DETECT
#define _NEW_FUN
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
//using XLibWrapper;
using HxCardDLL;
using System.Threading;
using OpenCvSharp;
using System.Net.Sockets;
using System.Net;
using Newtonsoft.Json;
using System.Diagnostics;

namespace XEthernetDemo
{
    public partial class TestForm : Form
    {
        #region  功放定义
        public struct Msg
        {
            internal byte[] bytes;
            internal int length;
            internal DateTime getTime;//这个有效
            internal DateTime saveTime;
            internal DateTime pickTime;
            internal DateTime processTime;
            internal DateTime sendTime;
            internal DateTime beforeDeTime;
        }

        public struct GFinfo
        {
            int _channelindex;
            Int64 _time;
            int _flag;
            bool _next_same;
            internal int channelindex
            {
                set
                {
                    //Console.WriteLine("物块在第{0}通道",value);
                    _channelindex = value;
                }
                get
                {
                    return _channelindex;
                }
            }          // 物块在第几通道
            internal Int64 time;                // 物块到达的时间
            internal int flag
            {
                set
                {
                    //Console.WriteLine("物块为：{0}", value);
                    _flag = value;
                }
                get
                {
                    return _flag;
                }
            }                  // 物块的类型
            internal bool next_same
            {
                set
                {
                    //Console.WriteLine("下一个物块是否相同：{0}", value);
                    _next_same = value;
                }
                get
                {
                    return _next_same;
                }
            }            // 下一个信息与自己时间信息是否相同
        }

        public struct GFList
        {
            internal GFinfo[] list;         // 所有时间相同的GFinfo元素
            internal int length;            // 表示列表中有效元素的个数
            internal bool is_active;        // 表示该列表是否是激活状态
            internal int start_channel;     // 列表开始的通道
            internal int end_channel;       // 列表结束的通道
        }

        class dataProcess          //功放处理数据
        {

            public int[] SData = new int[2048];
            int processway = 2;   //1:仅仅处理SCA

            //数据一
            public int Arridx, Chnidx, IfcVerMajor, IfcVerMinor;
            public int DppAvailable, SpectralDataAvailabel, SnapshotStatisticDataValid, MxPowerSupplyState, MxPowerSupplyStable, SddTemperatureReady;
            public int ChannelState, CntCommunicationReset, CntUdpPacket, CntUdpSendErr, CntMissedPacket, CntSyncErr, CntSnapshotStatInvalid;
            public float DppTemperature, SddTemperature;
            public int[] res = new int[7];

            //数据二
            public int SpectCtr, Events, BytesPerBin, NumberOfBins, StartBin;
            public float DppTransferTime, Realtime, Lifetime, InputCountRate, OutputCountRate, DeadtimeRatio;

            //数据三
            public int TemperatureWarning, TemperatureFault, MxPowerUndervoltageLockout, MixedModeDetected;
            public float MxCpuTemperature, MxCpuUpperSurfaceTemperature, MxCpuUnderSurfaceTemperature, MxCpuLoadCore1, MxCpuLoadCore2, MxPowerTemperature;

            //数据五
            //public int SCAStartBin ,SCALength ,SCACount;
            public int[] SCAStartBin = new int[5];
            public int[] SCALength = new int[5];
            public int[] SCACount = new int[5];


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

            public int[] ChannelMetaSection(ref int index, int SectionLen, string rawdata)
            {
                if (processway == 1)
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

                    res[0] = Chnidx;
                    res[1] = DppAvailable;
                    res[2] = SpectralDataAvailabel;
                    res[3] = SnapshotStatisticDataValid;
                    res[4] = MxPowerSupplyState;
                    res[5] = MxPowerSupplyStable;
                    res[6] = SddTemperatureReady;
                }


                return res;

            }

            public void SCADataSection(ref int index, int SectionLen, string rawdata)
            {
                int tempstartbin, templength, tempcount;
                for (int i = 0; i < 16; i++)
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
                    if (i >= 0 && i < 5)             //根据SCA个数要进行修改
                    {
                        SCAStartBin[i] = tempstartbin;
                        SCALength[i] = templength;
                        SCACount[i] = tempcount;
                    }

                }

            }

            public float SpectralMetaSection(ref int index, int SectionLen, string rawdata)
            {
                if (processway == 1)
                {
                    index += 70;
                    /*
                    index += 20;
                    string temp;
                    temp = rawdata.Substring(index, 8);
                    Lifetime = strToFloat(temp);
                    index += 50;
                    */
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
                return Lifetime;

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

        #endregion

        public TestForm()
        {
            InitializeComponent();
            //PsColor.SelectedIndex = 0;
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(CurrentDomain_UnhandleException);
        }


        private void TestForm_Load(object sender, EventArgs e)
        {
            Version.Text = version;
            selfDefParam();
        }

        private static void CurrentDomain_UnhandleException(object sender, UnhandledExceptionEventArgs e)
        {
            Console.WriteLine(e.ExceptionObject.ToString());
            MessageBox.Show(e.ExceptionObject.ToString());
        }

        public string version;

        //功放变量
        static Queue<Msg> msg_queue;
        static Queue<GFinfo> info_queue;
        static Socket socket;
        static AutoResetEvent conditional_variable;
        static Object gflocker;
        static volatile bool quit_flag = false;
        static IPEndPoint ep;

        static char[] idx_to_hex = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
        /*
        public int[,,] SCAData = new int[1000000, 10, 10];    //40为一秒轮次，可存储25秒数据
        string[,] DataGetTime = new string[1000000, 10];
        string[,] processTime = new string[1000000, 10];
        string[,] sendTime = new string[1000000, 10];
        string[,] saveTime = new string[1000000, 10];
        string[,] pickTime = new string[1000000, 10];
        string[,] beforeDeTime = new string[1000, 10];
        */
        public Boolean writeFlag = false;

        private int RunFlag = 0;                   //是否运行标志
        public int udpCnt = 1;
        static Socket udpRecv;
        static Socket udpSend;
        static IPEndPoint endpoint;
        static IPEndPoint sendpoint;


        public int CycleCount = 0;
        public int chnldx;
        public string getTime = "";
        public string currenTime = "";
        public int count = 0;
        public int gapsum = 0;


        public int DataLen = 0;
        public int msgcount = 0;
        public int sendflag = 1;  //是否发送udp数据
        public int intocount = 0;
        public int sendcount = 0;
        public int gap = 8;        //阈值 铜锌
        public int gap_pb = 15;     // 阈值 铅         
        public int[] recvData = new int[7];
        public int[] channelStateData = new int[10];

        //private Thread listen_thread;
        //private Thread process_thread1, process_thread2, process_thread3, process_thread4;

        // url
        //public Controller Conupdate = new Controller();
        //public ConSettings ConSet = new ConSettings();
        //public ConInformation ConInfo = new ConInformation();
        //public Channel Channelupdate = new Channel();
        //public ChannelInformation channInfo = new ChannelInformation();
        //public ChannelSettings channSet = new ChannelSettings();
        //public Parset parsetinfo = new Parset();
        //public List<SingleChannelAnalyzerConfigItem> Singleset = new List<SingleChannelAnalyzerConfigItem>();
        //public List<double> peaktime = new List<double>();
        //public DateTime starttime;

        //功放线阵交互
        public int opFlag = 0;      //是否正在对队列进行操作
        public bool recv = false;   //是否开始接受功放程序数据

        //线阵变量
        //XSystemW xsystem;
        //XDeviceW xdevice;
        //XGigFactoryW xfactory;
        //XCommandW xcommand;
        //XFrameTransferW xtransfer;
        //XAcquisitionW xacquisition;
        //XDisplayW xdisplay;
        //XOffCorrectW xcorrect;
        //XTifFormatW xtifform;

        HxCard hxCard;
        XRayMsgDLL.XRayPowerAmplifier xRayPower;

        int frame_count = 0;
        int lost_line = 0;
        int total_card_num = 0;
        uint cur_dm_index = 0;
        int total_clock_num = 0;                // 共检测到物块
        int total_detect_num = 0;               // 共喷吹物块
        Int64 time_now;
        Int64 time_finish;
        int pic_num = 0;
        int check_time_num = 0;                 // 定时器1的计数器
        int check_time_num2 = 0;                // 定时器2的计数器
        float integral_time = 0.5F;             // 默认扫描积分时间3ms
        public int num_of_mouth = 198;          // 喷嘴数量
        public int length_belt = 1016;          // 皮带长度为1000mm
        public int length_linearray = 1120;     // 线阵长度为1200mm
        public int AmplifierSetNum = 5;
        public int AmplifierEnableNum = 10;
        uint time_interval = 5;                 // 定时器定时时间为5ms
        public float speed = 3.0f;              // 传送带速度
        public float SDD = 914;
        public float SOD = 815;
        public float t1 = 524;
        public float t2 = 492;
        public float p1 = 616;
        public float p2 = 584;
        public float offestPercentage = 524 / 1016;
        public float l1 = 0;
        public float l2 = 0;
        string result_pic;
        string init_pic;
        Socket client = null;                   //与PLC连接的socket
        Socket client2 = null;                  //与功放程序连接的socket
        public byte[] Rcvbuffer;                //接收字节组数
        delegate void AppendDelegate(string str);
        AppendDelegate AppendString;
        string test_txt_filepath = "C:/Users/96342/Desktop/TEST20.txt";
        string result_data = System.Windows.Forms.Application.StartupPath + "/result/";
        string time_data = System.Windows.Forms.Application.StartupPath + "/result/";
        FileStream fs;
        //StreamWriter wr;
        FileStream fs2;
        //StreamWriter wr2;
#if _TEST
        public string ntpServer = "127.0.0.7";          // PLC IP
        public string arrayServer = "127.0.0.1";       // 线阵本地IP
        public string arrayRemoteIp = "127.0.0.2";      // 线阵远端IP
        public int arrayRemoteCmdPort = 3000;      // 线阵远端指令接收端口
        public int arrayRemoteDataPort = 4001;     // 线阵远端信息发送端口
        public int arrayLocalRecvPort = 4001;      // 线阵本地信息接收端口
        public string powerServer = "127.0.0.10";        // 功放IP
        public string ntpServer2 = "127.0.0.11";
        public string powerRemote = "127.0.0.22";
#else
        public string ntpServer = "192.168.250.1";          // PLC IP
        public string arrayServer = "192.168.0.10";       // 线阵本地IP
        public string arrayRemoteIp = "192.168.0.7";      // 线阵远端IP
        public int arrayRemoteCmdPort = 3000;      // 线阵远端指令接收端口
        public int arrayRemoteDataPort = 4001;     // 线阵远端信息发送端口
        public int arrayLocalRecvPort = 4001;      // 线阵本地信息接收端口
        public string powerServer = "172.28.110.50";        // 功放IP
        public string ntpServer2 = "127.0.0.1";
        public string powerRemote = "172.28.110.100";
#endif
        //OmronFinsNet omronFinisNet = new OmronFinsNet("192.168.250.1", 6001);
        //System.Timers.Timer t = new System.Timers.Timer(5);
        Thread timerthread;
        Thread recv_thread;
        static Object locker=new object();
        GFinfo first_info = new GFinfo();
        GFList gflist = new GFList();
        GFList gflist2 = new GFList();

        // 功能选择
        bool Select_Cu = true;
        bool Select_Zn = true;
        bool Select_Pb = false;
        bool Select_Fe = false;

        private void funtion_select()
        {
            Select_Cu = FunctionSelect_Cu.Checked;
            Select_Zn = FunctionSelect_Zn.Checked;
            Select_Pb = FunctionSelect_Pb.Checked;
            Select_Fe = FunctionSelect_Fe.Checked;
        }


        // 清空原本result里的pic信息
        private bool DeleteFiles(string path)
        {
            if (Directory.Exists(path) == false)
            {
                Error.Text = "pic dic not exits!";
                Directory.CreateDirectory(path);
                return false;
            }
            DirectoryInfo dir = new DirectoryInfo(path);
            FileInfo[] files = dir.GetFiles();
            try
            {
                foreach (var item in files)
                {
                    File.Delete(item.FullName);
                }
                if (dir.GetDirectories().Length != 0)
                {
                    foreach (var item in dir.GetDirectories())
                    {
                        if (!item.ToString().Contains("$") && (!item.ToString().Contains("Boot")))
                        {
                            DeleteFiles(dir.ToString() + "\\" + item.ToString());
                        }
                    }
                }
                //Directory.Delete(path);
                return true;
            }
            catch (Exception)
            {
                Error.Text = "Delete Failed!";
                return false;

            }
        }


        // 功放信息首列表清空并重置
        private void gflist_Reset()
        {
            Array.Clear(gflist.list, 0, gflist.list.Length);    // 先清空列表
            int i = 0;
            gflist.start_channel = 0;
            gflist.end_channel = 0;
            if (info_queue.Count != 0)
            {
                gflist.is_active = true;
                lock (locker)
                {
                    while (info_queue.Count != 0)
                    {
                        gflist.list[i] = info_queue.Dequeue();
                        if (gflist.start_channel == 0 && gflist.end_channel == 0)
                        {
                            gflist.start_channel = gflist.list[i].channelindex - 1;
                            gflist.end_channel = gflist.list[i].channelindex + 1;
                        }
                        if (gflist.list[i].channelindex <= gflist.start_channel)
                        {
                            gflist.start_channel = gflist.list[i].channelindex - 1;
                        }
                        if (gflist.list[i].channelindex >= gflist.end_channel)
                        {
                            gflist.end_channel = gflist.list[i].channelindex + 1;
                        }
                        if (gflist.start_channel < 1)
                            gflist.start_channel = 1;
                        if (gflist.end_channel > 10)
                            gflist.end_channel = 10;
                        if (!gflist.list[i].next_same)
                            break;
                        else
                            i++;
                    }
                }
                gflist.length = i + 1;
            }
            else
                gflist.length = 0;
            ////wr.WriteLine("Update the first time info list! The length of list is: " + (gflist.length) + "(" + gflist.start_channel + "," + gflist.end_channel + ")," + ",queue_count = " + info_queue.Count);
        }

        void Connect_to_PLC()
        {
            if (client == null)
            {
                client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            }
            if (!client.Connected)
            {
                // OperateResult connect = omronFinisNet.ConnectServer();
                // Data_Set syn_data = new Data_Set

                IPAddress ip = IPAddress.Parse(ntpServer);
                IPEndPoint remoteep = new IPEndPoint(ip, 6000);
                // AsyncCallback callback = new AsyncCallback(ConnectCallback);
                client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                client.Connect(remoteep);
                /*if (client.Connected)
                    Total_Block_Num.Text = "Successfully Connect!";
                else
                    Total_Block_Num.Text = "Unsuccessfully Connect!";
                client.ReceiveTimeout = 3000;*/
                // client.Send(ntp_testdata);
            }
        }

        void SetIntegralTime()
        {
            //ulong pixel_size;
            //unsafe
            //{
            //    ulong data;
            //    xcommand.GetPara(36, &data, 0);
            //    pixel_size = data;
            //}
            //float integral = 0;
            //integral = ((float)pixel_size / 10) / (speed * SDD / SOD);
            ////MessageBox.Show("The pixel size is:" + pixel_size + "/10mm;The integral time is:" + integral + "ms");
            //ulong integral_times = (ulong)(integral * 1000);
            //xcommand.SetPara(3, integral_times, 0);
            int integral = (int)(800 / speed);//us 像素长度/皮带运行速度
            hxCard.SetIntegrationTime((uint)integral);
            integral_time = (float)integral/1000;
        }

        // 向PLC发送消息
        public int SendData(Data_Set data)
        {
            if (client == null)
            {
                client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            }
            if (!client.Connected)
            {
                // OperateResult connect = omronFinisNet.ConnectServer();
                byte[] ntp_data = new byte[23];             // 整个数据包一共23byte

                // 将数据转换成byte类型
                ntp_data[0] = data.syn_code;                // 同步码
                byte[] a = new byte[4];
                a = Encoding.UTF8.GetBytes(data.flow_num);
                for (int i = 0; i < 4; i++)
                    ntp_data[1 + i] = a[i];                 // 流水编号
                ntp_data[5] = data.typof_block;             // 物块类型
                ntp_data[6] = Convert.ToByte(data.blow);    // 是否吹气
                byte[] b = new byte[8];
                /*
                for (int i = 0; i < 6; i++)
                    ntp_data[i + 7] = data.start_time[i];    // 开始吹气时间
                c = BitConverter.GetBytes(data.millionseconds);
                ntp_data[13] = c[0];
                ntp_data[14] = c[1];                        // 开始吹起的毫秒数
                */
                for (int i = 0; i < 8; i++)
                    ntp_data[i + 7] = (byte)(data.start_time_int >> (i * 8) & 0xff);    // 开始吹气时间
                byte[] c = new byte[2];
                c = BitConverter.GetBytes(data.blow_time);
                ntp_data[15] = c[0];
                ntp_data[16] = c[1];                        // 吹气持续时间
                c = BitConverter.GetBytes(data.start_num);
                ntp_data[17] = c[0];
                ntp_data[18] = c[1];                        // 开始吹气阀号
                c = BitConverter.GetBytes(data.end_num);
                ntp_data[19] = c[0];
                ntp_data[20] = c[1];                        // 开始吹气阀号
                ntp_data[21] = data.reserve;                // 保留值
                ntp_data[22] = data.check_num;              // 校验值

                IPAddress ip = IPAddress.Parse(ntpServer);
                IPEndPoint remoteep = new IPEndPoint(ip, 6000);
                // AsyncCallback callback = new AsyncCallback(ConnectCallback);
                client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                client.Connect(remoteep);
                client.ReceiveTimeout = 3000;
                return client.Send(ntp_data);
                // client.Receive(ntp_testdata);
                /*if (connect.IsSuccess)
                    MessageBox.Show("连接成功!");
                else
                    MessageBox.Show("连接失败!");*/
            }
            else
            {
                byte[] ntp_data = new byte[23];             // 整个数据包一共23byte

                // 将数据转换成byte类型
                ntp_data[0] = data.syn_code;                // 同步码
                byte[] a = new byte[4];
                a = Encoding.UTF8.GetBytes(data.flow_num);
                for (int i = 0; i < 4; i++)
                    ntp_data[1 + i] = a[i];                 // 流水编号
                ntp_data[5] = data.typof_block;             // 物块类型
                ntp_data[6] = Convert.ToByte(data.blow);    // 是否吹气
                byte[] b = new byte[8];
                /*
                for (int i = 0; i < 6; i++)
                    ntp_data[i + 7] = data.start_time[i];    // 开始吹气时间
                c = BitConverter.GetBytes(data.millionseconds);
                ntp_data[13] = c[0];
                ntp_data[14] = c[1];                        // 开始吹起的毫秒数
                */
                for (int i = 0; i < 7; i++)
                    ntp_data[i + 7] = (byte)(data.start_time_int >> (i * 8) & 0xff);    // 开始吹气时间
                byte[] c = new byte[2];
                c = BitConverter.GetBytes(data.blow_time);
                ntp_data[15] = c[0];
                ntp_data[16] = c[1];                        // 吹气持续时间
                //Console.WriteLine("blow time is:" + ntp_data[15] + "  " + ntp_data[16]);
                c = BitConverter.GetBytes(data.start_num);
                ntp_data[17] = c[0];
                ntp_data[18] = c[1];                        // 开始吹气阀号
                if (c[1] != 0)
                    Console.WriteLine("开始吹气阀号：{0}", c[1]);
                c = BitConverter.GetBytes(data.end_num);
                ntp_data[19] = c[0];
                ntp_data[20] = c[1];                        // 开始吹气阀号
                ntp_data[21] = data.reserve;                // 保留值
                ntp_data[22] = data.check_num;              // 校验值

                client.ReceiveTimeout = 3000;
                return client.Send(ntp_data);
            }
        }

        // 定时检测功放当前通道功能是否正常
        //public void ChannelStateCheck()
        //{
        //    if (process_thread1.IsAlive)
        //    {
        //        for(int i = 0; i < channelStateData.Length; i++)
        //        {
        //            if (channelStateData[i] != 1)
        //                OnError(50, "Channel " + i + " error!");
        //        }
        //    }
        //    //wr2.WriteLine(DateTime.Now.ToString() + "ChannelStateCheck Finishing!");
        //}

        // 发送当前时间点的时间戳
        public void timecheck()
        {
            DateTime stamp = DateTime.Now;
            Data_Set data = new Data_Set();
            data.check_num = 1;     // 此时数据包为检验数据包，进行时间同步
            check_time_num++;

            TimeSpan time_stamp = stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            data.start_time_int = (Int64)time_stamp.TotalMilliseconds;

            int num = SendData(data);
            //if (num == 23 && check_time_num % 50 == 0)
            if (num == 23)
            {
                //CardNum1.Text = "Successfully AutoCheckTime!";
                //Total_Block_Num.Text =  Convert.ToString(stamp) + ":" + Convert.ToString(stamp.Millisecond) + "ms";
                //Total_Block_Num.Text = Convert.ToString(data.start_time_int) + "ms";
                //Total_Block_Num.Text = "check_time_num is " + Convert.ToString(check_time_num);
                //Total_Block_Num.Text = "Unsuccessfully send check info!The check_time_num is: " + Convert.ToString(check_time_num);
                //timeBox1.Text = Convert.ToString(DateTime.Now) + Convert.ToString(DateTime.Now.Millisecond);
            }
            if (data.start_time_int % 10000 == 0)
            {
                //wr.WriteLine("Check!" + '\t' + Convert.ToString(data.start_time_int) + '\t');
                //wr.Flush();
            }
        }

        // 获得格林威治时间函数
        private uint timeGetTime()
        {
            TimeSpan time_stamp = DateTime.Now - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return (uint)time_stamp.TotalMilliseconds;
        }

        // 定时器功能实现函数
        private void timer()
        {
            uint timestart = timeGetTime();
            while (true)
            {
                uint i = 0;
                while (i < time_interval)
                {
                    i = timeGetTime() - timestart;
                }
                timestart = timeGetTime();
                timecheck();
            }
        }

        // 接收功放数据函数
        private void open_recv()
        {
            if (client2 == null)
            {
                client2 = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            }
            if (!client2.Connected)
            {
                // OperateResult connect = omronFinisNet.ConnectServer();
                // Data_Set syn_data = new Data_Set

                IPAddress ip = IPAddress.Parse(ntpServer2);
                IPEndPoint remoteep = new IPEndPoint(ip, 1130);
                // AsyncCallback callback = new AsyncCallback(ConnectCallback);
                client2 = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                client2.Bind(remoteep);
                //client2.ReceiveTimeout = 3000;
            }
        }

        // 启动接受功放程序数据函数
        // 启动接受功放程序数据函数
        private void recv_data()
        {
            byte[] data = new byte[100];
            while (recv)
            {
                //EndPoint remoteEndPoint = new IPEndPoint(IPAddress.Any, 0);
                try
                {
                    int length = client2.Receive(data);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                    Console.WriteLine(ex.ToString());
                    continue;
                }
                //int length = client2.ReceiveFrom(data, ref remoteEndPoint);
                /*if (length != 10)
                {
                    Total_Block_Num.Text = "Receive the Wrong Message!";
                    continue;
                }*/
                // 解析时间和通道数
                GFinfo info = new GFinfo();
                Int64 time = Convert.ToInt64("0", 2);
                string a = "";
                for (int i = 7; i >= 0; i--)
                {
                    a = a + data[i].ToString("X2");
                }
                time = Convert.ToInt64(a, 16);
                info.time = Convert.ToInt64(time.ToString()) - 15;
                //index = index << 8 & data[9];
                info.channelindex = Convert.ToInt32(data[8].ToString("X2"), 16);
                info.flag = 1;               // 入队列的为铜的信息
                info.next_same = false;     // 初始化下一个物块时间信息与自己是不同的
                // 检测当前进队列的物块时间信息和前一个物块是否相同
                lock (locker)
                {
                    if (info_queue.Count != 0)
                    {
                        GFinfo[] list = info_queue.ToArray<GFinfo>();
                        GFinfo last_info = list[list.Length - 1];
                        if (Math.Abs(last_info.time - info.time) <= 5)
                        {
                            
                            list[list.Length - 1].next_same = true;
                            Console.WriteLine("功率放中有物块！");
                        }
                        else
                        {
                            if(list.Length < 2)
                            {
                                list = new GFinfo[0];
                            }
                        }

                        info_queue = new Queue<GFinfo>(list);
                    }
                    info_queue.Enqueue(info);
                }
                try
                {
                    //wr.WriteLine("Receive time: " + info.time.ToString() + ",index: " + info.channelindex.ToString());
                    //wr.Flush();
                }
                catch(ObjectDisposedException ex)
                {
                    Error.Text = "Error: " + ex.Message;
                }
                

            }
            if (!recv)
            {
                //Total_Block_Num.Text = "Stop Receive!";
                Thread.CurrentThread.Abort();
            }

        }

        private void recv_data_Pro(object sender, EventArgs e)
        {
            XRayMsgDLL.XRayPowerAmplifier.XRayFrame frame = e as XRayMsgDLL.XRayPowerAmplifier.XRayFrame;
            for (int j = 0; j < frame.ResponseArea.Length; j++) 
            {
                int channelGet = frame.ResponseArea[j];
                if(channelGet == 2)
                {
                    // 解析时间和通道数
                    GFinfo info = new GFinfo();
                    Int64 time = frame.Time;
                    info.time = time - 35;
                    //index = index << 8 & data[9];

                    info.channelindex = j + 1;// Convert.ToInt32(data[8].ToString("X2"), 16);
                    info.flag = 1;               // 入队列的为铜的信息
                    info.next_same = false;     // 初始化下一个物块时间信息与自己是不同的
                                                // 检测当前进队列的物块时间信息和前一个物块是否相同
                    lock (locker)
                    {
                        if (info_queue.Count != 0)
                        {
                            GFinfo[] list = info_queue.ToArray<GFinfo>();
                            GFinfo last_info = list[list.Length - 1];
                            if (Math.Abs(last_info.time - info.time) <= 5)
                            {

                                list[list.Length - 1].next_same = true;
                                Console.WriteLine("功率放中有物块！");
                            }
                            else
                            {
                                if (list.Length < 2)
                                {
                                    list = new GFinfo[0];
                                }
                            }

                            info_queue = new Queue<GFinfo>(list);
                        }
                        info_queue.Enqueue(info);
                    }
                    try
                    {
                        //wr.WriteLine("Receive time: " + info.time.ToString() + ",index: " + info.channelindex.ToString());
                        //wr.Flush();
                    }
                    catch (ObjectDisposedException ex)
                    {
                        Error.Text = "Error: " + ex.Message;
                    }
                }

            }
           
        }

        private void recv_data_SimplePro(object sender, EventArgs e)
        {
            XRayMsgDLL.XRayPowerAmplifier.SimpleFrame frame = e as XRayMsgDLL.XRayPowerAmplifier.SimpleFrame;
            // 解析时间和通道数
            GFinfo info = new GFinfo();
            Int64 time = frame.Time;
            info.time = time - 21;
            //index = index << 8 & data[9];

            info.channelindex = (int)((frame.Channel + 1) * (float)AmplifierSetNum / AmplifierEnableNum);// Convert.ToInt32(data[8].ToString("X2"), 16);
            info.flag = 1;               // 入队列的为铜的信息
            info.next_same = false;     // 初始化下一个物块时间信息与自己是不同的
                                        // 检测当前进队列的物块时间信息和前一个物块是否相同
            lock (locker)
            {
                if (info_queue.Count != 0)
                {
                    GFinfo[] list = info_queue.ToArray<GFinfo>();
                    GFinfo last_info = list[list.Length - 1];
                    if (Math.Abs(last_info.time - info.time) <= 5)
                    {

                        list[list.Length - 1].next_same = true;
                        Console.WriteLine("功率放中有物块！");
                    }
                    else
                    {
                        if (list.Length < 2)
                        {
                            list = new GFinfo[0];
                        }
                    }

                    info_queue = new Queue<GFinfo>(list);
                }
                info_queue.Enqueue(info);
            }
            try
            {
                //wr.WriteLine("Receive time: " + info.time.ToString() + ",index: " + info.channelindex.ToString());
                //wr.Flush();
            }
            catch (ObjectDisposedException ex)
            {
                Error.Text = "Error: " + ex.Message;
            }

        }

        // 三个错误显示程序
        void OnError(int err_id, string msg)
        {
            Error.Text = "Error ID: " + err_id.ToString() + "，" + msg;
            /*
            if (timerthread.IsAlive)
            {
                if (StopButton.Enabled)
                {
                    StopButton_Click(msg, EventArgs.Empty);
                }
                else
                {
                    timerthread.Abort();
                }
            }
            */
        }
        void OnEvent1(int event_id, int data)
        {
            System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = false;
            switch (event_id)
            {
                case 50:
                case 53:
                    lost_line += data;
                    LostLine.Text = "Lost Line: " + Convert.ToString(lost_line);
                    break;
            }
        }
        void OnEvent2(int event_id, float data)
        {
            System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = false;
            switch (event_id)
            {
                case 56:
                    Temperature.Text = "Temperature: " + Convert.ToString(data);
                    break;
                case 57:
                    Humidity.Text = "Humidity: " + Convert.ToString(data);
                    break;
            }
        }

        private void Power_Amplifier_Load()
        {
            msg_queue = new Queue<Msg>();
            info_queue = new Queue<GFinfo>();
            conditional_variable = new AutoResetEvent(false);
            gflocker = new object();
            xRayPower = new XRayMsgDLL.XRayPowerAmplifier(2, true, AmplifierEnableNum);
            //xRayPower.RegisterFrameReadyCallback(recv_data_Pro);
            xRayPower.RegisterSimpleReadyCallback(recv_data_SimplePro);
            xRayPower.Connect(powerServer, powerRemote, 27001, 19200);
            //udpRecv = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);                     //接收功放设备udp数据报   25ms10个包（10个通道每个通道25ms发一个包）
            //endpoint = new IPEndPoint(IPAddress.Parse(powerServer), 27001);          //172.28.110.50   27001
            //udpRecv.Bind(endpoint);
            //
            //udpSend = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);                    //给HJ负责设备发送数据报
            //sendpoint = new IPEndPoint(IPAddress.Parse(ntpServer2), 1130);
            ////udpSend.Bind(sendpoint);
            //udpSend.Connect(sendpoint);

        }

        void OnFrameReady(HxCard.XImgFrame[] images)
        {
            frame_count++;
            threadCounters(images);
            //xdisplay.Display(image);
        }

        private void Humidity_Click(object sender, EventArgs e)
        {

        }
#if _TEST
        private void StartButton_Click(object sender, EventArgs e)
        {
            xRayPower.Start();
            paraSetButton.Enabled = false;
            DeleteFiles(result_data + "pic/");
            frame_count = 0;
            lost_line = 0;
            total_clock_num = 0;
            total_detect_num = 0;
            DateTime time = DateTime.Now;
            fs = new FileStream(result_data + "result_data " + time.ToString("yyyy-MM-dd") + " " + time.Hour + "-" + time.Minute + "-" + time.Second + ".txt", FileMode.Append);
            fs2 = new FileStream(time_data + "frame-time-data " + time.ToString("yyyy-MM-dd") + " " + time.Hour + "-" + time.Minute + "-" + time.Second + ".txt", FileMode.Append);
            DateTime dt = DateTime.Now;

            LostLine.Text = "Lost Line: " + Convert.ToString(lost_line);
            Console.WriteLine("start grab!!!");

            // 设定定时器
            timerthread = new Thread(timer);
            timerthread.IsBackground = true;
            timerthread.Start();

            // 启动接收功放数据线程
            recv = true;
            if (recv)
            {
                //CardNum1.Text = "Start Receive!";
                recv_thread = new Thread(recv_data);
                recv_thread.IsBackground = true;
                recv_thread.Start();
            }

            first_info.flag = 0;
            first_info.channelindex = 0;
            first_info.time = 0;

            gflist.is_active = false;       // 初始设置保存队列中最早时间功放信息列表激活值为false，表示列表暂时未启用或需要重新刷新列表值
            gflist.list = new GFinfo[100];
            gflist.length = 0;
            gflist2.is_active = false;       // 初始设置保存队列中最早时间功放信息列表激活值为false，表示列表暂时未启用或需要重新刷新列表值
            gflist2.list = new GFinfo[100];
            gflist2.length = 0;

            quit_flag = false;
            //listen_thread = new Thread(RecvMessage);
            //process_thread1 = new Thread(ProcessMessage);
            

            //listen_thread.IsBackground = true;
            //process_thread1.IsBackground = true;
            //listen_thread.Start();
            //process_thread1.Start();

            // 功放功能定时检测
            //ChannelChecktimer.Enabled = true;

            //MessageBox.Show("成功启动线阵与功放！");
            StartButton.Enabled = false;
            StopButton.Enabled = true;
        }
#else
        private void StartButton_Click(object sender, EventArgs e)
        {
            System.Threading.Thread.Sleep(5);
            xRayPower.Start();
            System.Threading.Thread.Sleep(5);
            hxCard.SetChannelNum(12);
            System.Threading.Thread.Sleep(5);
            SetIntegralTime();
            System.Threading.Thread.Sleep(5);
            hxCard.SetGain(62);
            System.Threading.Thread.Sleep(5);
            hxCard.SetPixelMode(1);
            System.Threading.Thread.Sleep(5);
            paraSetButton.Enabled = false;
            DeleteFiles(result_data + "pic/");
            frame_count = 0;
            lost_line = 0;
            total_clock_num = 0;
            total_detect_num = 0;
            DateTime time = DateTime.Now;
            fs = new FileStream(result_data + "result_data " + time.ToString("yyyy-MM-dd") + " " + time.Hour + "-" + time.Minute + "-" + time.Second + ".txt", FileMode.Append);
            //wr = new StreamWriter(fs);
            fs2 = new FileStream(time_data + "frame-time-data " + time.ToString("yyyy-MM-dd") + " " + time.Hour + "-" + time.Minute + "-" + time.Second + ".txt", FileMode.Append);
            //wr2 = new StreamWriter(fs2);
            //wr.WriteLine("\n*************************************************************************");
            DateTime dt = DateTime.Now;
            //wr.WriteLine("*******************" + Convert.ToString(dt) + "********************");
            //wr2.WriteLine("\n*************************************************************************");
            //wr2.WriteLine("*******************" + Convert.ToString(dt) + "********************");
            //wr.WriteLine("frame_count" + '\t' + "start_num" + '\t' + "end_num" + '\t' + "start_time" + '\t' + '\t' + "blow_time" + '\t' + '\t' + "send_time\ttime1\ttime2\ttypof_block\tqueue_flag\tarea\tblack_location");
            //wr.Flush();
            //wr2.WriteLine("frame_count\tcontour_length\tstart_X\tstart_Y\tWidth\tHeight");
            //wr2.Flush();
            LostLine.Text = "Lost Line: " + Convert.ToString(lost_line);
            //xacquisition.Grab(0);
            hxCard.StartSampling();
            Console.WriteLine("start grab!!!");

            // 设定定时器
            timerthread = new Thread(timer);
            timerthread.IsBackground = true;
            timerthread.Start();

            // 启动接收功放数据线程
            recv = true;
            if (recv)
            {
                //CardNum1.Text = "Start Receive!";
                recv_thread = new Thread(recv_data);
                recv_thread.IsBackground = true;
                recv_thread.Start();
            }

            first_info.flag = 0;
            first_info.channelindex = 0;
            first_info.time = 0;

            gflist.is_active = false;       // 初始设置保存队列中最早时间功放信息列表激活值为false，表示列表暂时未启用或需要重新刷新列表值
            gflist.list = new GFinfo[100];
            gflist.length = 0;
            gflist2.is_active = false;       // 初始设置保存队列中最早时间功放信息列表激活值为false，表示列表暂时未启用或需要重新刷新列表值
            gflist2.list = new GFinfo[100];
            gflist2.length = 0;

            quit_flag = false;
#if _OLD
            listen_thread = new Thread(RecvMessage);
            process_thread1 = new Thread(ProcessMessage);
            

            listen_thread.IsBackground = true;
            process_thread1.IsBackground = true;
            listen_thread.Start();
            process_thread1.Start();
#endif
            // 功放功能定时检测
            //ChannelChecktimer.Enabled = true;

            //MessageBox.Show("成功启动线阵与功放！");
            StartButton.Enabled = false;
            StopButton.Enabled = true;
        }
#endif
        void threadCounters(Object obj)
        {
            DateTime stamp = DateTime.Now;
            HxCard.XImgFrame[] images = (HxCard.XImgFrame[])obj;
            pic_num++;
            //int value = Marshal.ReadInt32(image.DataAddr, 0);
            HxCard.XImgFrame image = images[0];
            try
            {
                time_now = DateTime.Now.Millisecond;
                uint[,] pixelval = new uint[image.Height, image.Width];
                uint maxp = 0;
                uint minp = 65536;
                int m, n, row, col;

                time_finish = DateTime.Now.Millisecond;

                Thread.Sleep(30);
                //getNewPicture(images);
                getCounters_Pixel(images[0], images[1], MatType.CV_16UC1, stamp);//image.HeadTime);
            }
            catch (ThreadAbortException e)
            {
                MessageBox.Show(e.Message);
            }

        }

        // 物块是否是铜的判断函数(五个点任意一点满足就可以)
        private int Is_Material(HxCard.XImgFrame ximagew, int X, int Y, int Width, int Height, int value)
        {
            /*
            if (ximagew.GetPixelVal((uint)(Y + Height / 4), (uint)(X + Width / 4)) < value)
            {
                return 100000 + (int)ximagew.GetPixelVal((uint)(Y + Height / 4), (uint)(X + Width / 4));
            }
            else if (ximagew.GetPixelVal((uint)(Y + Height / 4), (uint)(X + Width / 2)) < value)
            {
                return 200000 + (int)ximagew.GetPixelVal((uint)(Y + Height / 4), (uint)(X + Width / 2));
            }
            else if (ximagew.GetPixelVal((uint)(Y + Height / 4), (uint)(X + Width * 3 / 4)) < value)
            {
                return 300000 + (int)ximagew.GetPixelVal((uint)(Y + Height / 4), (uint)(X + Width * 3 / 4));
            }
            else if (ximagew.GetPixelVal((uint)(Y + Height / 2), (uint)(X + Width / 4)) < value)
            {
                return 400000 + (int)ximagew.GetPixelVal((uint)(Y + Height / 2), (uint)(X + Width / 4));
            }
            else if (ximagew.GetPixelVal((uint)(Y + Height / 2), (uint)(X + Width / 2)) < value)
            {
                return 500000 + (int)ximagew.GetPixelVal((uint)(Y + Height / 2), (uint)(X + Width / 2));
            }
            else if (ximagew.GetPixelVal((uint)(Y + Height / 2), (uint)(X + Width * 3 / 4)) < value)
            {
                return 600000 + (int)ximagew.GetPixelVal((uint)(Y + Height / 2), (uint)(X + Width * 3 / 4));
            }
            else if (ximagew.GetPixelVal((uint)(Y + Height * 3 / 4), (uint)(X + Width / 4)) < value)
            {
                return 700000 + (int)ximagew.GetPixelVal((uint)(Y + Height * 3 / 4), (uint)(X + Width / 4));
            }
            else if (ximagew.GetPixelVal((uint)(Y + Height * 3 / 4), (uint)(X + Width / 2)) < value)
            {
                return 800000 + (int)ximagew.GetPixelVal((uint)(Y + Height * 3 / 4), (uint)(X + Width / 2));
            }
            else if (ximagew.GetPixelVal((uint)(Y + Height * 3 / 4), (uint)(X + Width * 3 / 4)) < value)
            {
                return 900000 + (int)ximagew.GetPixelVal((uint)(Y + Height * 3 / 4), (uint)(X + Width * 3 / 4));
            }
            else
                return 0;
            */
            for(int i = X; i <= X + Width; i += 2)
            {
                for(int j = Y; j <= Y + Height; j += 2)
                {
                    uint k = ximagew.GetPixelVal((uint)j, (uint)i);
                    if (k <= value)
                        return (int)k;
                }
            }
            return 0;

        }

        int imgIndex = 0;
        int checkRange = 20;
        Stopwatch sw = new Stopwatch();
        int minItemArea = 20;
        int itemGrayThreshold = 80;
        int subItemGrayThresh = 60;
        int subItemAreaThresh = 20;
        int grayMeanDiffThresh = 50;
        int trueItemCunt = 0;
        int choosedItemCunt = 0;
        int totalItemCunt = 0;
        List<Data_Set> sendedMsgList = new List<Data_Set>();
        int check = 0;

        public void getCounters_Pixel(HxCard.XImgFrame maskImage,HxCard.XImgFrame rawImage, MatType type, DateTime stamp)
        {
            int lslItemCunt = 0;

            int row = (int)maskImage.Height;
            int col = (int)maskImage.Width;
            Mat img = new Mat(new int[] { row, col }, MatType.CV_8UC1, maskImage.Data);
            time_now = DateTime.Now.Millisecond;
            Mat maskImg = img.Clone();
            Mat rawImg = new Mat(new int[] { row, col }, MatType.CV_8UC1, rawImage.Data);
#if _DETECT
            //img.ConvertTo(img, MatType.CV_32F);
            //Cv2.Normalize(img, img, 1.0, 0, NormTypes.MinMax);
            //img = img * 255;
            //img.ConvertTo(img, MatType.CV_8UC1);
            ////Cv2.ImWrite(init_pic, image);
            //Mat connImage = new Mat(100, 100, MatType.CV_8UC3, new Scalar(0, 0, 0));
            //Cv2.Blur(img, img, new OpenCvSharp.Size(3, 3));
            Mat element = Cv2.GetStructuringElement(MorphShapes.Rect, new OpenCvSharp.Size(3, 3));               //定义核，开运算
            Cv2.MorphologyEx(img, img, MorphTypes.Open, element);                    //进行形态学开运算操作
            element = Cv2.GetStructuringElement(MorphShapes.Rect, new OpenCvSharp.Size(3, 3));               //定义核，开运算
            Cv2.MorphologyEx(img, img, MorphTypes.Close, element);                    //进行形态学闭运算操作
            //Cv2.CvtColor(image, image, ColorConversionCodes.BGR2GRAY); // 具体看输入图像为几通道，单通道则注释
            Cv2.Canny(img, img, 10, 80, 3, false); // 输入图像虚为单通道8位图像
            OpenCvSharp.Point[][] contours;
            HierarchyIndex[] hierarchy;
            Cv2.FindContours(img, out contours, out hierarchy, RetrievalModes.External, ContourApproximationModes.ApproxSimple);
#endif
#if _IO
            Mat imgRgb = new Mat(new int[] { row, col }, MatType.CV_8UC3);
            Cv2.CvtColor(rawImg, imgRgb, ColorConversionCodes.GRAY2BGR);
#endif


            Rect[] boundRect = new Rect[contours.Length];
            int[] lineMetalType = new int[contours.Length];
#if _DETECT
            if (true)// contours.Length > 0)
            {
                List<deepTestItem> deepResultList = new List<deepTestItem>();
                List<ManualResetEvent> arrManul = new List<ManualResetEvent>();

                for (int index = 0; index < contours.Length; index++)
                {
                    boundRect[index] = Cv2.BoundingRect(contours[index]);                //获取每个contour的框
                    //"(hierarchy[i].Child < 0 && boundRect[i].TopLeft.Y == 0)"部分用于判断是否有跨页的物块被误识别，判断原理：https://stackoverflow.com/questions/22240746/recognize-open-and-closed-shapes-opencv
                    if (boundRect[index].TopLeft.Y > maskImage.RoiHeight || (hierarchy[index].Child < 0 && boundRect[index].TopLeft.Y == 0)) 
                        continue;
                    var subItemRect = Cv2.BoundingRect(contours[index]);
                    double area = subItemRect.Height * subItemRect.Width;//Cv2.ContourArea(contours[index]);

                    if (area < minItemArea )//|| boundRect[index].Height > row / 3)
                    {
#if _IO
                        Cv2.PutText(imgRgb, String.Format("area{0}:{1}", index, area.ToString("f1")),
                                    new OpenCvSharp.Point(boundRect[index].X + 10, boundRect[index].Y-20),
                                    HersheyFonts.HersheySimplex, 0.3, new Scalar(0, 0, 255));
                        Cv2.DrawContours(imgRgb, contours, index, new Scalar(0, 0, 255), 1);
#endif
                        continue;
                    }
                    else
                    {
#if _IO
                        Cv2.DrawContours(imgRgb, contours, index, new Scalar(255, 0, 0), 1);
#endif
                    }

                    //Cv2.DrawContours(imgRgb, contours, index, new Scalar(0, 0, 255), 1);
#if _IO
                    OpenCvSharp.Point point1 = new OpenCvSharp.Point(boundRect[index].BottomRight.X, boundRect[index].BottomRight.Y);
                    OpenCvSharp.Point point2 = new OpenCvSharp.Point(boundRect[index].TopLeft.X, boundRect[index].TopLeft.Y);
                    Cv2.Rectangle(imgRgb, point2, point1, 100, 1);
#endif
                    Mat mask = maskImg[boundRect[index]];
                    Mat tinyItem = rawImg[boundRect[index]];                        //用框裁剪出所有contour的最小mat,生成一个mat[]
                    Mat gloryItem = tinyItem.Clone();
                    Cv2.BitwiseAnd(tinyItem, mask, gloryItem);
#if _DEEP
                    deepTestItem dlItem = new deepTestItem(tinyItem, index);
                    dlItem.manual = new ManualResetEvent(false);
                    arrManul.Add(dlItem.manual);
                    if (area > 200)
                    {
                        ThreadPool.QueueUserWorkItem(new WaitCallback(deepLearnClassificationAysn), dlItem);
#if _IO
                        Cv2.Rectangle(imgRgb, point2, point1, new Scalar(0, 255, 255), 1);
#endif
                    }
                    else
                    {
                        dlItem.manual.Set();
                    }
                    deepResultList.Add(dlItem);
#endif
                    area = Cv2.CountNonZero(gloryItem);
                    Scalar sumValue = Cv2.Sum(gloryItem);
                    double mean = sumValue.Val0 / area;                         //计算每个mat的平均值

                    double subArea = 0;
                    double totalGray = 0;
                    double subMean = 0;
                    bool luosi = findBlockInItemGrayDiff(tinyItem, subItemGrayThresh, subItemAreaThresh, imgIndex,index, out subArea, out totalGray);
                    double outMean = 0;
                    outMean = (sumValue.Val0 - totalGray) / (area - subArea);
                    subMean = subArea == 0 ? outMean : totalGray / subArea;
                    double MeanDiff = Math.Abs(outMean - subMean);
                    int flag = 0;
                    trueItemCunt++;
                    if (mean < itemGrayThreshold || luosi || (MeanDiff > grayMeanDiffThresh && subArea > 25)) 
                    {
                        choosedItemCunt++;
                        lineMetalType[index] = 1;
#if _IO
                        Cv2.Rectangle(imgRgb, point2, point1, new Scalar(0, 0, 255), 5);
#endif
                        if(mean < itemGrayThreshold)
                        {
                            flag += 4;
                        }
                        if(luosi)
                        {
                            flag += 2;
                        }
                        if(MeanDiff > grayMeanDiffThresh && subArea > 25)
                        {
                            flag += 1;
                        }
                        //choosedItemCunt++;
                    }
                    else
                    {
                        lineMetalType[index] = 0;
                    }
#if _IO
                    Cv2.PutText(imgRgb, String.Format("mean{0}:{1},area:{2},trick:{3}", index, mean.ToString("f1"), area,Convert.ToString(flag,2)),
            new OpenCvSharp.Point(boundRect[index].X + 10, boundRect[index].Y - 20),
            HersheyFonts.HersheySimplex, 0.3, new Scalar(125, 125, 0));
#endif
                    //Cv2.PutText(imgRgb, String.Format("area{0}:{1}", index, area.ToString("f1")),
                    //            new OpenCvSharp.Point(boundRect[index].X + 10, boundRect[index].Y),
                    //            HersheyFonts.HersheySimplex, 0.3, new Scalar(0, 255, 0));
                    //Console.WriteLine("Mean: {0}+{1}={2}", sumValue.Val0.ToString("f1"), area.ToString("f1"), mean.ToString("f1"));
                    totalItemCunt++;
                    lslItemCunt++;
                }



                int tickCunt = 0;
#if _DEEP
                bool deepLearnCheck = false;
                if (lslItemCunt > 0) 
                {
                    int batch = arrManul.Count / 64;
                    int tail = arrManul.Count % 64;
                    for (int j = 0; j < batch; j++)
                    {
                        deepLearnCheck = WaitHandle.WaitAll(arrManul.Skip(j * 64).Take(64).ToArray(), 130);
                    }
                    deepLearnCheck = WaitHandle.WaitAll(arrManul.Skip(batch * 64).Take(tail).ToArray(), 130);
                }
                foreach (var item in deepResultList)
                {
                    if(item.result < 0.5)
                    {
                        if (lineMetalType[item.index] != 1)
                            choosedItemCunt++;
                        lineMetalType[item.index] = 1;
#if _IO
                        OpenCvSharp.Point point1 = new OpenCvSharp.Point(boundRect[item.index].BottomRight.X, boundRect[item.index].BottomRight.Y);
                        OpenCvSharp.Point point2 = new OpenCvSharp.Point(boundRect[item.index].TopLeft.X, boundRect[item.index].TopLeft.Y);
                        Cv2.Rectangle(imgRgb, point2, point1, new Scalar(0, 255, 255), 5);
#endif

                    }
                }
#endif
                sw.Start();
                for (int j = 0; j < contours.Length; j++)
                {

                    int i = contours.Length - j - 1;



                    //if (boundRect[i].BottomRight.Y > maskImage.RoiHeight)
                    //    continue;
                    //if (contours.Length > 50)
                    //break;
                    //double area = Cv2.ContourArea(contours[i]);
                    //if (area == 0 || boundRect[i].Width < 5 || boundRect[i].Height < 4)
                    //    continue;

                    //wr2?.WriteLine(Convert.ToString((stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + '\t' + contours.Length + '\t' + boundRect[i].X + '\t' + boundRect[i].Y + '\t' + boundRect[i].Width + '\t' + boundRect[i].Height);
                    //341 347 348 346


                    Data_Set data = new Data_Set();                                 // 发送数据包
                    // data.flow_num = Convert.ToString(total_card_num % 1000);        // 设置流水编号
                    data.Init_Dataset(boundRect[i], maskImage);                       // 初始化数据包为可吹气  

                    bool is_small = false;


                    // 设置开始喷吹时间(使用格林威治毫秒时间)
                    int k = (int)((boundRect[i].Y * integral_time) - integral_time * maskImage.MaxHeight);  // 找出物块在图像中的实际时间
                    TimeSpan time_stamp = stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0);
                    data.start_time_int = (Int64)time_stamp.TotalMilliseconds;
                    data.start_time_int = data.start_time_int + k;                                      // 计算物块到达X光的时间
                    long start_detect_time = data.start_time_int;


                    DateTime time1 = DateTime.Now;

#if _NEW_FUN
                    // 设置开始吹气阀号和停止吹气阀号
                    float startPixel = LineConvert2Belt((float)boundRect[i].X / col * (p1 + p2));
                    float endPixel = LineConvert2Belt((float)(boundRect[i].X + boundRect[i].Width) / col * (p1 + p2));
                    data.start_num = PixelConvert2Num(startPixel);
                    data.end_num = PixelConvert2Num(endPixel);
#else
                    // 设置开始吹气阀号和停止吹气阀号
                    if ((float)boundRect[i].X / col > 0.5)
                    {
                        data.start_num = (Int16)((((float)boundRect[i].X / col * length_linearray - (length_linearray / 2)) * (float)SOD / SDD + (length_belt / 2)) / length_belt * num_of_mouth);
                        data.end_num = (Int16)(((((float)boundRect[i].X + boundRect[i].Width) / col * length_linearray - (length_linearray / 2)) * (float)SOD / SDD + (length_belt / 2)) / length_belt * num_of_mouth);
                    }
                    else
                    {
                        data.start_num = (Int16)(((length_belt / 2) - ((length_linearray / 2) - (float)boundRect[i].X / col * length_linearray) * (float)SOD / SDD) / length_belt * num_of_mouth - 1);
                        if ((float)boundRect[i].X + boundRect[i].Width > col / 2)
                            data.end_num = (Int16)(((((float)boundRect[i].X + boundRect[i].Width) / col * length_linearray - (length_linearray / 2)) * (float)SOD / SDD + (length_belt / 2)) / length_belt * num_of_mouth);
                        else
                            data.end_num = (Int16)(((length_belt / 2) - ((length_linearray / 2) - ((float)boundRect[i].X + boundRect[i].Width) / col * length_linearray) * (float)SOD / SDD) / length_belt * num_of_mouth - 1);
                    }
#endif

                    if (data.start_num < 1)
                        data.start_num = (short)1;
                    data.end_num = (Int16)(data.start_num + (float)boundRect[i].Width / col * length_linearray * (float)SOD / SDD / length_belt * num_of_mouth + 1);

                    if (data.end_num > num_of_mouth)
                        data.end_num = (short)num_of_mouth;


                    int queue_flag = 0;                         // 标志当前队列第一个是否与物块信息符合,默认为最新金属还没轮到当前物块
                    int start_num = (int)Math.Ceiling((float)data.start_num * AmplifierSetNum / num_of_mouth);
                    int end_num = (int)Math.Ceiling((float)data.end_num * AmplifierSetNum / num_of_mouth);
                    data.typof_block = (byte)lineMetalType[i];
                    if (data.typof_block == 1)
                    {
                        check++;
                    }

                    while (true)
                    {
                        if (!gflist.is_active && info_queue.Count != 0)
                        {
                            gflist_Reset();
                        }
                        if (!gflist.is_active)
                            break;
                        for (int n = 0; n < gflist.length; n++)
                        {
                            first_info = gflist.list[n];
                            Int64 a = data.start_time_int - first_info.time;
                            Console.WriteLine("a:" + a);
                            Console.WriteLine("========");
                            if (Math.Abs(a) <= checkRange)
                            {
                                if (gflist.start_channel <= end_num && gflist.end_channel >= start_num)
                                {
                                    queue_flag = 1;
                                    data.typof_block = 1;//(byte)first_info.flag;
                                    break;
                                }
                                else
                                {
                                    queue_flag = 3;
                                    continue;
                                }
                            }
                            else if (a > checkRange)    // 如果当前物块时间已经大于当前队列中第一个物块的时间
                            {
                                queue_flag = 2;
                                break;
                            }
                            else                // 如果当前物块时间远小于列表时间
                            {
                                queue_flag = 0;
                                break;
                            }
                        }
                        if (queue_flag == 1)        // 如果队列信息已经被读取，需要对队列首个物块进行剔除
                        {
                            break;
                        }
                        else if (queue_flag == 2)                   // 如果列表中金属信息时间远小于当前物块时间，需要更新列表信息
                        {
                            lock (locker)
                            {
                                gflist.is_active = false;        // 如果队列信息已经过期，将当前列表设为未激活状态，下一个循环会当前列表信息更新
                            }
                        }
                        else if (queue_flag == 3)               // 如果当前列表时间符合但通道数不符合，则说明当前物块不是需要喷吹的目标金属，跳出循环
                        {
                            break;
                        }
                        else                                    // 当前物块时间远小于列表时间，则说明当前物块不是需要喷吹的目标金属，跳出循环
                        {
                            break;
                        }

                    }
#if _IO
                    #region CZQ
                    int itemButtomDown = TimeConvert2Index((Int64)(stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds, data.start_time_int - checkRange);
                    int itemButtomUp = TimeConvert2Index((Int64)(stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds, data.start_time_int + checkRange);
                    int itemLeft = (int)(BeltConvert2Line((float)(start_num - 1) / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width);
                    int itemRight = (int)(BeltConvert2Line((float)end_num / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width);
                    //Cv2.Rectangle(imgRgb, new OpenCvSharp.Point(itemLeft, itemButtomDown), new OpenCvSharp.Point(itemRight, itemButtomUp), 100, 5);

                    int AmpLineY = TimeConvert2Index((Int64)(stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds, first_info.time);
                    int AmpLineLeft = (int)(BeltConvert2Line((float)(gflist.start_channel - 1) / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width);//gflist.start_channel
                    //int AmpLineLeft = (int)((float)(gflist.start_channel - 1) / AmplifierSetNum * image.Width);//gflist.start_channel
                    //int AmpLineRight = (int)((float)(gflist.end_channel) / AmplifierSetNum * image.Width);//gflist.start_channel
                    int AmpLineRight = (int)(BeltConvert2Line((float)(gflist.end_channel) / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width);//gflist.start_channel
                    Cv2.Line(imgRgb, new OpenCvSharp.Point(AmpLineLeft, AmpLineY), new OpenCvSharp.Point(AmpLineRight, AmpLineY), 100, 1);
                    //Console.WriteLine("first_info Time:{0},AmpLine P1:{1},AmpLine P2:{2}", AmpLineY, new OpenCvSharp.Point(AmpLineLeft, AmpLineY), new OpenCvSharp.Point(AmpLineRight, AmpLineY));

                    //Console.WriteLine("Get Rect,{0},{1}", new OpenCvSharp.Point(itemLeft, itemButtomDown), new OpenCvSharp.Point(itemRight, itemButtomUp));
                    //Console.WriteLine("图像中有物块！");

                    #endregion
#endif
                    // 确定物块最终喷吹的时间
                    data.start_time_int += (int)(2400 / speed) - 13;                                    // 计算出物块到达喷嘴的格林威治毫秒时间


                    //data.start_time = (Int64)stamp.TotalMilliseconds + (Int64)(boundRect[i].Y / line_num_persecond);
                    // 设置持续喷吹的时间
                    data.blow_time = (Int16)(boundRect[i].Height * integral_time + 7);
                    //data.blow_time = (short)100;


                    // 让延迟一段时间发送物块信息 
                    // Thread.Sleep((int)(2400/speed) - );
                    // Thread t = new Thread(new ParameterizedThreadStart(thread_for_sending));
                    // //wr.WriteLine(t.ManagedThreadId + " thread start!");
                    // t.Start();

                    // 每个物块休眠2ms时间让PLC来得及处理物块信息
                    //if (i != 0)
                    //Thread.Sleep(2);
                    int num = 0;
                    //if ((data.typof_block == 1 && Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Height, boundRect[i].Width, 8000) > 0) || (is_small && Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Height, boundRect[i].Width, 8000) > 0))
                    // Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Width, boundRect[i].Height, 6000);
                    //Console.WriteLine("Checked?:{0}", FunctionSelect_NoSelect.Checked);
                    //Console.WriteLine("Metal_Type?:{0}", (data.typof_block == 1));
                    //Console.WriteLine("Rect?:{0}", (boundRect[i].Y >= row * 0.9));
                    //Console.WriteLine("kk?:{0}", kk > 0);

                    if (FunctionSelect_NoSelect.Checked || (data.typof_block == 1)) //|| (boundRect[i].Y >= row * 0.9 && kk > 0))
                    {

                        num = SendData(data);
                        sendedMsgList.Add(data);
                        Console.WriteLine("SendData");
                        if ((data.typof_block == 1) == true)
                            Console.WriteLine("物块识别喷吹");
                        if ((boundRect[i].Y >= row * 0.9) == true)
                            Console.WriteLine("物块跨行喷吹");
                        Console.WriteLine("***********************************");

                        data.typof_block = 1;
#if _IO
                        Cv2.PutText(imgRgb, "blow", new OpenCvSharp.Point(boundRect[i].X + 10, boundRect[i].Y),
                        HersheyFonts.HersheySimplex, 0.3, new Scalar(0, 255, 0));
#endif
                    }

                    DateTime time2 = DateTime.Now;

                    total_clock_num++;
                    if (num == 23)
                    {
                        total_detect_num++;
                        //CardNum1.Text = total_detect_num + "Detect / " + total_clock_num + " Find";
                        //CardNum2.Text = "start:" + Convert.ToString(data.start_num) + "," + "end:" + Convert.ToString(data.end_num);
                        //CardNum3.Text = Convert.ToString(data.start_time_int) + "ms " + k;
                        //CardNum4.Text = Convert.ToString(DateTime.Now.Millisecond - stamp.Millisecond) + "ms";
                        //CardNum5.Text = Convert.ToString(integral_time * 512) + "ms per picture";
                    }

                    if (num == 23 && is_small)
                        queue_flag = 4;
                    //wr?.WriteLine(Convert.ToString(frame_count) + " " + i.ToString() + '\t' + Convert.ToString(data.start_num) + " " + start_num + '\t' + Convert.ToString(data.end_num) + " " + end_num + '\t' + start_detect_time + '\t' + data.blow_time + "ms\t" + Convert.ToString((DateTime.Now - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + "\t" + Convert.ToString((time1 - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + '\t' + Convert.ToString((time2 - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + '\t' + data.typof_block + "\t" + queue_flag + "\t" + area + "\t" + kk.ToString());
                    //Console.WriteLine();
                }
                sw.Stop();
            }

#endif
            //sw.Stop();
            //Console.WriteLine(sw.ElapsedMilliseconds);
            //sw.Reset();

            //time_finish = DateTime.Now.Millisecond;

            if (contours.Length != 0)
            {
                // Total_Block_Num.Text = Convert.ToString(total_card_num);
                // 画出检测的轮廓
                int flag = 0;
                //Cv2.ImWrite(result_pic, connImage);

                // 求出时间戳并发送物块信息


                #region CZQ
#if _IO
                OpenCvSharp.Point point1 = new OpenCvSharp.Point(maskImage.Width, maskImage.RoiHeight);
                OpenCvSharp.Point point2 = new OpenCvSharp.Point(0, 0);
                Cv2.Rectangle(imgRgb, point2, point1, 100, 1);
                for (int ii = 0; ii <= AmplifierSetNum; ii++)
                {
                    Cv2.Line(imgRgb, new OpenCvSharp.Point(BeltConvert2Line((float)ii / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width, 0),
                        new OpenCvSharp.Point(BeltConvert2Line((float)ii / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width, hxCard.ImgHeight), 100, 1);
                }
                Bitmap bitmapImg = OpenCvSharp.Extensions.BitmapConverter.ToBitmap(imgRgb);
                DrawImg(bitmapImg);
                bitmapImg.Save(".\\测试1\\" + imgIndex + ".bmp");
#endif
                imgIndex++;
                #endregion

            }

            // ushort[,] line_info = get_timestamp_test(ximagew);
            //Console.WriteLine(ximagew.DataOffset);

            element.Release();
            element.Dispose();
            rawImg.Release();
            rawImg.Dispose();
            maskImg.Release();
            maskImg.Dispose();
            img.Release();
            img.Dispose();
#if _IO
            imgRgb.Release();
            imgRgb.Dispose();
#endif
            if (sw.ElapsedMilliseconds > maxTime)
                maxTime = sw.ElapsedMilliseconds;
            Console.WriteLine("Img Time:{0}| MaxTime:{1}", sw.ElapsedMilliseconds, maxTime);
            sw.Reset();
        }
        long maxTime = 0;
        public void getCounters_Pixel_Asyn(HxCard.XImgFrame maskImage, HxCard.XImgFrame rawImage, MatType type, DateTime stamp)
        {
            int lslItemCunt = 0;



            int row = (int)maskImage.Height;
            int col = (int)maskImage.Width;
            Mat img = new Mat(new int[] { row, col }, MatType.CV_8UC1, maskImage.Data);
            time_now = DateTime.Now.Millisecond;
            Mat maskImg = img.Clone();
            Mat rawImg = new Mat(new int[] { row, col }, MatType.CV_8UC1, rawImage.Data);
#if _DETECT
            //img.ConvertTo(img, MatType.CV_32F);
            //Cv2.Normalize(img, img, 1.0, 0, NormTypes.MinMax);
            //img = img * 255;
            //img.ConvertTo(img, MatType.CV_8UC1);
            ////Cv2.ImWrite(init_pic, image);
            //Mat connImage = new Mat(100, 100, MatType.CV_8UC3, new Scalar(0, 0, 0));
            //Cv2.Blur(img, img, new OpenCvSharp.Size(3, 3));
            Mat element = Cv2.GetStructuringElement(MorphShapes.Rect, new OpenCvSharp.Size(3, 3));               //定义核，开运算
            Cv2.MorphologyEx(img, img, MorphTypes.Open, element);                    //进行形态学开运算操作
            element = Cv2.GetStructuringElement(MorphShapes.Rect, new OpenCvSharp.Size(3, 3));               //定义核，开运算
            Cv2.MorphologyEx(img, img, MorphTypes.Close, element);                    //进行形态学闭运算操作
            //Cv2.CvtColor(image, image, ColorConversionCodes.BGR2GRAY); // 具体看输入图像为几通道，单通道则注释
            Cv2.Canny(img, img, 10, 80, 3, false); // 输入图像虚为单通道8位图像
            OpenCvSharp.Point[][] contours;
            HierarchyIndex[] hierarchy;
            Cv2.FindContours(img, out contours, out hierarchy, RetrievalModes.External, ContourApproximationModes.ApproxSimple);
#endif
#if _IO
            Mat imgRgb = new Mat(new int[] { row, col }, MatType.CV_8UC3);
            Cv2.CvtColor(rawImg, imgRgb, ColorConversionCodes.GRAY2BGR);
#endif


            Rect[] boundRect = new Rect[contours.Length];
            int[] lineMetalType = new int[contours.Length];
            sw.Start();
#if _DETECT
            if (true)// contours.Length > 0)
            {
                List<interVariableSet> deepResultList = new List<interVariableSet>();
                List<ManualResetEvent> arrManul = new List<ManualResetEvent>();

                for (int index = 0; index < contours.Length; index++)
                {
                    interVariableSet variable = new interVariableSet(boundRect, contours, index, (int)(rawImage.RoiHeight), maskImg, rawImg, hierarchy, lineMetalType);
                    arrManul.Add(variable.manual);
                    //itemClassification(variable);
                    ThreadPool.QueueUserWorkItem(new WaitCallback(itemClassification), variable);
                    deepResultList.Add(variable);
                }
                bool deepLearnCheck = false;
                if (arrManul.Count > 0)
                {
                    int batch = arrManul.Count / 64;
                    int tail = arrManul.Count % 64;
                    for (int j = 0; j < batch; j++)
                    {
                        deepLearnCheck = WaitHandle.WaitAll(arrManul.Skip(j * 64).Take(64).ToArray(), 130);
                    }
                    deepLearnCheck = WaitHandle.WaitAll(arrManul.Skip(batch * 64).Take(tail).ToArray(), 130);
                }

                int tickCunt = 0;


                for (int j = 0; j < contours.Length; j++)
                {

                    int i = contours.Length - j - 1;
                    if (boundRect[i].BottomRight.Y > maskImage.RoiHeight)
                        continue;
                    //if (contours.Length > 50)
                    //break;
                    //double area = Cv2.ContourArea(contours[i]);
                    //if (area == 0 || boundRect[i].Width < 5 || boundRect[i].Height < 4)
                    //    continue;

                    //wr2?.WriteLine(Convert.ToString((stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + '\t' + contours.Length + '\t' + boundRect[i].X + '\t' + boundRect[i].Y + '\t' + boundRect[i].Width + '\t' + boundRect[i].Height);
                    //341 347 348 346


                    Data_Set data = new Data_Set();                                 // 发送数据包
                    // data.flow_num = Convert.ToString(total_card_num % 1000);        // 设置流水编号
                    data.Init_Dataset(boundRect[i], maskImage);                       // 初始化数据包为可吹气  

                    bool is_small = false;


                    // 设置开始喷吹时间(使用格林威治毫秒时间)
                    int k = (int)((boundRect[i].Y * integral_time) - integral_time * maskImage.MaxHeight);  // 找出物块在图像中的实际时间
                    TimeSpan time_stamp = stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0);
                    data.start_time_int = (Int64)time_stamp.TotalMilliseconds;
                    data.start_time_int = data.start_time_int + k;                                      // 计算物块到达X光的时间
                    long start_detect_time = data.start_time_int;


                    DateTime time1 = DateTime.Now;

#if _NEW_FUN
                    // 设置开始吹气阀号和停止吹气阀号
                    float startPixel = LineConvert2Belt((float)boundRect[i].X / col * (p1 + p2));
                    float endPixel = LineConvert2Belt((float)(boundRect[i].X + boundRect[i].Width) / col * (p1 + p2));
                    data.start_num = PixelConvert2Num(startPixel);
                    data.end_num = PixelConvert2Num(endPixel);
#else
                    // 设置开始吹气阀号和停止吹气阀号
                    if ((float)boundRect[i].X / col > 0.5)
                    {
                        data.start_num = (Int16)((((float)boundRect[i].X / col * length_linearray - (length_linearray / 2)) * (float)SOD / SDD + (length_belt / 2)) / length_belt * num_of_mouth);
                        data.end_num = (Int16)(((((float)boundRect[i].X + boundRect[i].Width) / col * length_linearray - (length_linearray / 2)) * (float)SOD / SDD + (length_belt / 2)) / length_belt * num_of_mouth);
                    }
                    else
                    {
                        data.start_num = (Int16)(((length_belt / 2) - ((length_linearray / 2) - (float)boundRect[i].X / col * length_linearray) * (float)SOD / SDD) / length_belt * num_of_mouth - 1);
                        if ((float)boundRect[i].X + boundRect[i].Width > col / 2)
                            data.end_num = (Int16)(((((float)boundRect[i].X + boundRect[i].Width) / col * length_linearray - (length_linearray / 2)) * (float)SOD / SDD + (length_belt / 2)) / length_belt * num_of_mouth);
                        else
                            data.end_num = (Int16)(((length_belt / 2) - ((length_linearray / 2) - ((float)boundRect[i].X + boundRect[i].Width) / col * length_linearray) * (float)SOD / SDD) / length_belt * num_of_mouth - 1);
                    }
#endif

                    if (data.start_num < 1)
                        data.start_num = (short)1;
                    data.end_num = (Int16)(data.start_num + (float)boundRect[i].Width / col * length_linearray * (float)SOD / SDD / length_belt * num_of_mouth + 1);

                    if (data.end_num > num_of_mouth)
                        data.end_num = (short)num_of_mouth;


                    int queue_flag = 0;                         // 标志当前队列第一个是否与物块信息符合,默认为最新金属还没轮到当前物块
                    int start_num = (int)Math.Ceiling((float)data.start_num * AmplifierSetNum / num_of_mouth);
                    int end_num = (int)Math.Ceiling((float)data.end_num * AmplifierSetNum / num_of_mouth);
                    data.typof_block = (byte)lineMetalType[i];


                    while (true)
                    {
                        if (!gflist.is_active && info_queue.Count != 0)
                        {
                            gflist_Reset();
                        }
                        if (!gflist.is_active)
                            break;
                        for (int n = 0; n < gflist.length; n++)
                        {
                            first_info = gflist.list[n];
                            Int64 a = data.start_time_int - first_info.time;
                            Console.WriteLine("a:" + a);
                            Console.WriteLine("========");
                            if (Math.Abs(a) <= checkRange)
                            {
                                if (gflist.start_channel <= end_num && gflist.end_channel >= start_num)
                                {
                                    queue_flag = 1;
                                    data.typof_block = (byte)first_info.flag;
                                    break;
                                }
                                else
                                {
                                    queue_flag = 3;
                                    continue;
                                }
                            }
                            else if (a > checkRange)    // 如果当前物块时间已经大于当前队列中第一个物块的时间
                            {
                                queue_flag = 2;
                                break;
                            }
                            else                // 如果当前物块时间远小于列表时间
                            {
                                queue_flag = 0;
                                break;
                            }
                        }
                        if (queue_flag == 1)        // 如果队列信息已经被读取，需要对队列首个物块进行剔除
                        {
                            break;
                        }
                        else if (queue_flag == 2)                   // 如果列表中金属信息时间远小于当前物块时间，需要更新列表信息
                        {
                            lock (locker)
                            {
                                gflist.is_active = false;        // 如果队列信息已经过期，将当前列表设为未激活状态，下一个循环会当前列表信息更新
                            }
                        }
                        else if (queue_flag == 3)               // 如果当前列表时间符合但通道数不符合，则说明当前物块不是需要喷吹的目标金属，跳出循环
                        {
                            break;
                        }
                        else                                    // 当前物块时间远小于列表时间，则说明当前物块不是需要喷吹的目标金属，跳出循环
                        {
                            break;
                        }

                    }
#if _IO
                    #region CZQ

                    //Console.WriteLine("Start Num：{0}，End Num：{1}", data.start_num, data.end_num);
                    //foreach (OpenCvSharp.Point[] contour in contours)
                    //{
                    //    //Cv2.Rectangle();
                    //    Rect bound = Cv2.BoundingRect(contour);
                    //    //int num = rect.Length;
                    //    //for(int i = 1;i<num;i++)
                    //    //{
                    //    //    Cv2.Line(image, rect[i - 1], rect[i], 255, 2);
                    //    //}
                    //    OpenCvSharp.Point point1 = new OpenCvSharp.Point(bound.BottomRight.X, bound.BottomRight.Y);
                    //    OpenCvSharp.Point point2 = new OpenCvSharp.Point(bound.TopLeft.X, bound.TopLeft.Y);
                    //    Cv2.Rectangle(imgRgb, point2, point1, 100, 2);
                    //    if (Math.Abs(point2.X - point1.X) > 10)
                    //    {
                    //        //Console.WriteLine("Rect -> P2:{0}", p2);
                    //        //Console.WriteLine("Rect -> P1:{0}", p1);
                    //    }
                    //}
                    int itemButtomDown = TimeConvert2Index((Int64)(stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds, data.start_time_int - checkRange);
                    int itemButtomUp = TimeConvert2Index((Int64)(stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds, data.start_time_int + checkRange);
                    int itemLeft = (int)(BeltConvert2Line((float)(start_num - 1) / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width);
                    int itemRight = (int)(BeltConvert2Line((float)end_num / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width);
                    //Cv2.Rectangle(imgRgb, new OpenCvSharp.Point(itemLeft, itemButtomDown), new OpenCvSharp.Point(itemRight, itemButtomUp), 100, 5);

                    int AmpLineY = TimeConvert2Index((Int64)(stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds, first_info.time);
                    int AmpLineLeft = (int)(BeltConvert2Line((float)(gflist.start_channel - 1) / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width);//gflist.start_channel
                    //int AmpLineLeft = (int)((float)(gflist.start_channel - 1) / AmplifierSetNum * image.Width);//gflist.start_channel
                    //int AmpLineRight = (int)((float)(gflist.end_channel) / AmplifierSetNum * image.Width);//gflist.start_channel
                    int AmpLineRight = (int)(BeltConvert2Line((float)(gflist.end_channel) / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width);//gflist.start_channel
                    Cv2.Line(imgRgb, new OpenCvSharp.Point(AmpLineLeft, AmpLineY), new OpenCvSharp.Point(AmpLineRight, AmpLineY), 100, 1);
                    //Console.WriteLine("first_info Time:{0},AmpLine P1:{1},AmpLine P2:{2}", AmpLineY, new OpenCvSharp.Point(AmpLineLeft, AmpLineY), new OpenCvSharp.Point(AmpLineRight, AmpLineY));

                    //Console.WriteLine("Get Rect,{0},{1}", new OpenCvSharp.Point(itemLeft, itemButtomDown), new OpenCvSharp.Point(itemRight, itemButtomUp));
                    //Console.WriteLine("图像中有物块！");

                    #endregion
#endif
                    // 确定物块最终喷吹的时间
                    data.start_time_int += (int)(2400 / speed) - 13;                                    // 计算出物块到达喷嘴的格林威治毫秒时间


                    //data.start_time = (Int64)stamp.TotalMilliseconds + (Int64)(boundRect[i].Y / line_num_persecond);
                    // 设置持续喷吹的时间
                    data.blow_time = (Int16)(boundRect[i].Height * integral_time + 7);
                    //data.blow_time = (short)100;


                    // 让延迟一段时间发送物块信息 
                    // Thread.Sleep((int)(2400/speed) - );
                    // Thread t = new Thread(new ParameterizedThreadStart(thread_for_sending));
                    // //wr.WriteLine(t.ManagedThreadId + " thread start!");
                    // t.Start();

                    // 每个物块休眠2ms时间让PLC来得及处理物块信息
                    //if (i != 0)
                    //Thread.Sleep(2);
                    int num = 0;
                    //if ((data.typof_block == 1 && Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Height, boundRect[i].Width, 8000) > 0) || (is_small && Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Height, boundRect[i].Width, 8000) > 0))
                    // Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Width, boundRect[i].Height, 6000);
                    //Console.WriteLine("Checked?:{0}", FunctionSelect_NoSelect.Checked);
                    //Console.WriteLine("Metal_Type?:{0}", (data.typof_block == 1));
                    //Console.WriteLine("Rect?:{0}", (boundRect[i].Y >= row * 0.9));
                    //Console.WriteLine("kk?:{0}", kk > 0);

                    if (FunctionSelect_NoSelect.Checked || (data.typof_block == 1)) //|| (boundRect[i].Y >= row * 0.9 && kk > 0))
                    {

                        num = SendData(data);
                        sendedMsgList.Add(data);
                        Console.WriteLine("SendData");
                        if ((data.typof_block == 1) == true)
                            Console.WriteLine("物块识别喷吹");
                        if ((boundRect[i].Y >= row * 0.9) == true)
                            Console.WriteLine("物块跨行喷吹");
                        Console.WriteLine("***********************************");

                        data.typof_block = 1;
#if _IO
                        Cv2.PutText(imgRgb, "blow", new OpenCvSharp.Point(boundRect[i].X + 10, boundRect[i].Y),
                        HersheyFonts.HersheySimplex, 0.3, new Scalar(0, 255, 0));
#endif
                    }

                    DateTime time2 = DateTime.Now;

                    total_clock_num++;
                    if (num == 23)
                    {
                        total_detect_num++;
                        //CardNum1.Text = total_detect_num + "Detect / " + total_clock_num + " Find";
                        //CardNum2.Text = "start:" + Convert.ToString(data.start_num) + "," + "end:" + Convert.ToString(data.end_num);
                        //CardNum3.Text = Convert.ToString(data.start_time_int) + "ms " + k;
                        //CardNum4.Text = Convert.ToString(DateTime.Now.Millisecond - stamp.Millisecond) + "ms";
                        //CardNum5.Text = Convert.ToString(integral_time * 512) + "ms per picture";
                    }

                    if (num == 23 && is_small)
                        queue_flag = 4;
                    //wr?.WriteLine(Convert.ToString(frame_count) + " " + i.ToString() + '\t' + Convert.ToString(data.start_num) + " " + start_num + '\t' + Convert.ToString(data.end_num) + " " + end_num + '\t' + start_detect_time + '\t' + data.blow_time + "ms\t" + Convert.ToString((DateTime.Now - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + "\t" + Convert.ToString((time1 - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + '\t' + Convert.ToString((time2 - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + '\t' + data.typof_block + "\t" + queue_flag + "\t" + area + "\t" + kk.ToString());
                    //Console.WriteLine();
                }

            }
            sw.Stop();

#endif
            //sw.Stop();
            //Console.WriteLine(sw.ElapsedMilliseconds);
            //sw.Reset();

            //time_finish = DateTime.Now.Millisecond;

            if (contours.Length != 0)
            {
                // Total_Block_Num.Text = Convert.ToString(total_card_num);
                // 画出检测的轮廓
                int flag = 0;
                //Cv2.ImWrite(result_pic, connImage);

                // 求出时间戳并发送物块信息


                #region CZQ
#if _IO
                OpenCvSharp.Point point1 = new OpenCvSharp.Point(maskImage.Width, maskImage.RoiHeight);
                OpenCvSharp.Point point2 = new OpenCvSharp.Point(0, 0);
                Cv2.Rectangle(imgRgb, point2, point1, 100, 1);
                for (int ii = 0; ii <= AmplifierSetNum; ii++)
                {
                    Cv2.Line(imgRgb, new OpenCvSharp.Point(BeltConvert2Line((float)ii / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width, 0),
                        new OpenCvSharp.Point(BeltConvert2Line((float)ii / AmplifierSetNum * (t1 + t2)) / (p1 + p2) * imgRgb.Width, hxCard.ImgHeight), 100, 1);
                }
                Bitmap bitmapImg = OpenCvSharp.Extensions.BitmapConverter.ToBitmap(imgRgb);
                DrawImg(bitmapImg);
                bitmapImg.Save(".\\测试1\\" + imgIndex + ".bmp");
#endif
                imgIndex++;
                #endregion

            }

            // ushort[,] line_info = get_timestamp_test(ximagew);
            //Console.WriteLine(ximagew.DataOffset);

            element.Release();
            element.Dispose();
            //rawImg.Release();
            //rawImg.Dispose();
            //maskImg.Release();
            //maskImg.Dispose();
            img.Release();
            img.Dispose();
#if _IO
            imgRgb.Release();
            imgRgb.Dispose();
#endif

            Console.WriteLine("Img Time:{0}| MaxTime:{1}", sw.ElapsedMilliseconds, maxTime);
            sw.Reset();
        }
        class deepTestItem
        {
            public ManualResetEvent manual;
            public Mat img;
            public string lab;
            public double result = 0;
            public int index;
            public deepTestItem(Mat m, int i)
            {
                img = m;
                index = i;
            }
        }
        public class interVariableSet
        {
            public Rect[] boundRect;
            public OpenCvSharp.Point[][] contours;
            public int index;
            public int roiHeight;
            public Mat maskImg;
            public Mat rawImg;
            public HierarchyIndex[] hierarchy;
            public ManualResetEvent manual;
            public int[] lineMetalType;
            public interVariableSet(Rect[] _boundRect, OpenCvSharp.Point[][] _contours, int _index,int _roiHeight, Mat _maskImg, Mat _rawImg,
                HierarchyIndex[] _hierarchy, int[] _lineMetalType)
            {
                boundRect = _boundRect;
                contours = _contours;
                index = _index;
                roiHeight = _roiHeight;
                maskImg = _maskImg;
                rawImg = _rawImg;
                hierarchy = _hierarchy;
                manual = new ManualResetEvent(false);
                contours = _contours;
                lineMetalType = _lineMetalType;
            }
        }

        void itemClassification(object o)
        {
            interVariableSet item = o as interVariableSet;
            item.boundRect[item.index] = Cv2.BoundingRect(item.contours[item.index]);                //获取每个contour的框
            //"(hierarchy[i].Child < 0 && boundRect[i].TopLeft.Y == 0)"部分用于判断是否有跨页的物块被误识别，判断原理：https://stackoverflow.com/questions/22240746/recognize-open-and-closed-shapes-opencv
            if (item.boundRect[item.index].TopLeft.Y > item.roiHeight || (item.hierarchy[item.index].Child < 0 && item.boundRect[item.index].TopLeft.Y == 0))
                return;
            var subItemRect = Cv2.BoundingRect(item.contours[item.index]);
            double area = subItemRect.Height * subItemRect.Width;//Cv2.ContourArea(contours[index]);

            if (area < minItemArea)//|| boundRect[index].Height > row / 3)
            {
                return;
            }

            //Cv2.DrawContours(imgRgb, contours, index, new Scalar(0, 0, 255), 1);

            Mat mask = item.maskImg[item.boundRect[item.index]];
            Mat tinyItem = item.rawImg[item.boundRect[item.index]];                        //用框裁剪出所有contour的最小mat,生成一个mat[]
            Mat gloryItem = tinyItem.Clone();
            Cv2.BitwiseAnd(tinyItem, mask, gloryItem);
#if _DEEP
            deepLearnClassification(tinyItem,out string label,out float result);
#endif

            area = Cv2.CountNonZero(gloryItem);
            Scalar sumValue = Cv2.Sum(gloryItem);
            double mean = sumValue.Val0 / area;                         //计算每个mat的平均值

            double subArea = 0;
            double totalGray = 0;
            double subMean = 0;
            bool luosi = findBlockInItemGrayDiff(tinyItem, subItemGrayThresh, subItemAreaThresh, imgIndex, item.index, out subArea, out totalGray);
            double outMean = 0;
            outMean = (sumValue.Val0 - totalGray) / (area - subArea);
            subMean = subArea == 0 ? outMean : totalGray / subArea;
            double MeanDiff = Math.Abs(outMean - subMean);
            int flag = 0;
            trueItemCunt++;
            if (mean < itemGrayThreshold || luosi || (MeanDiff > grayMeanDiffThresh && subArea > 25))
            {
                choosedItemCunt++;
                item.lineMetalType[item.index] = 1;
                if (mean < itemGrayThreshold)
                {
                    flag += 4;
                }
                if (luosi)
                {
                    flag += 2;
                }
                if (MeanDiff > grayMeanDiffThresh && subArea > 25)
                {
                    flag += 1;
                }
                //choosedItemCunt++;
            }
            else
            {
                item.lineMetalType[item.index] = 0;
            }
            //Cv2.PutText(imgRgb, String.Format("area{0}:{1}", index, area.ToString("f1")),
            //            new OpenCvSharp.Point(boundRect[index].X + 10, boundRect[index].Y),
            //            HersheyFonts.HersheySimplex, 0.3, new Scalar(0, 255, 0));
            //Console.WriteLine("Mean: {0}+{1}={2}", sumValue.Val0.ToString("f1"), area.ToString("f1"), mean.ToString("f1"));
            totalItemCunt++;
            item.manual.Set();
        }

        void deepLearnClassificationAysn(object o)
        {
            deepTestItem tinyImgs = o as deepTestItem;
            Stopwatch sw = new Stopwatch();
            sw.Start();
            var deepLearnResult = User.InferenceModel(tinyImgs.img.Clone());
            tinyImgs.lab = deepLearnResult.Label;
            tinyImgs.result = deepLearnResult.Confidence;
            sw.Stop();
            Console.WriteLine("DeepLearnTime:{0}", sw.ElapsedMilliseconds);
            sw.Reset();
            //for (int i = 0; i < 1000; i++)
            //    for (int j = 0; j < 1000; j++) ;
            tinyImgs.manual.Set();
        }
        void deepLearnClassification(Mat img,out string label,out float result)
        {
            var deepLearnResult = User.InferenceModel(img.Clone());
            label = deepLearnResult.Label;
            result = deepLearnResult.Confidence;
        }

        private bool findBlockInItem(Mat item, int grayThresh, int areaThresh)
        {
            bool ret = false;
            Mat threshItem = item.Clone();
            Cv2.Threshold(item, threshItem, grayThresh, 255, ThresholdTypes.BinaryInv);

            OpenCvSharp.Point[][] contours;
            HierarchyIndex[] hierarchy;
            Cv2.FindContours(threshItem, out contours, out hierarchy, RetrievalModes.External, ContourApproximationModes.ApproxSimple);
            int i = 0;
            double totalArea = 0;
            foreach (OpenCvSharp.Point[] contour in contours)
            {
                double area = Cv2.ContourArea(contour);
                var boundingRect = Cv2.BoundingRect(contour);
                if (boundingRect.Height < item.Height / 2 && boundingRect.Width < item.Width / 2)
                    totalArea += area;
                i++;
            }
            if (totalArea > areaThresh)
                ret = true;
            return ret;
        }
        private bool findBlockInItemGrayDiff(Mat item, int grayThresh, int areaThresh, int imgIndex, int itemIndex, out double totalArea, out double totalGray)
        {
            bool ret = false;
            Mat threshItem = item.Clone();
            Cv2.Threshold(item, threshItem, grayThresh, 255, ThresholdTypes.BinaryInv);
            //Cv2.AdaptiveThreshold(item, threshItem, 255, AdaptiveThresholdTypes.GaussianC, ThresholdTypes.BinaryInv, 3, 0);
            OpenCvSharp.Point[][] contours;
            HierarchyIndex[] hierarchy;
            Cv2.FindContours(threshItem, out contours, out hierarchy, RetrievalModes.External, ContourApproximationModes.ApproxSimple);
            totalArea = 0;

            //for (int i = 0; i < contours.Length; i++)
            //{
            //    var contour = contours[i];
            //
            //    totalArea += area;
            //}
            double invArea = 0;
            for (int i = 0; i < contours.Length; i++)
            {
                var contour = contours[i];
                var boundingRect = Cv2.BoundingRect(contour);
                if (!(boundingRect.Height < item.Height / 2 && boundingRect.Width < item.Width / 2))
                {
                    invArea += Cv2.CountNonZero(threshItem[boundingRect]);
                }

            }
            totalArea = Cv2.CountNonZero(threshItem);
            var subTotalArea = totalArea - invArea;
            Mat gloryItem = threshItem.Clone();
            Cv2.BitwiseAnd(item, threshItem, gloryItem);
            Scalar sumValue = Cv2.Sum(gloryItem);

            totalGray = sumValue.Val0;                         //计算每个mat的平均值

            if (subTotalArea > areaThresh)
                ret = true;
            //gradientTest(item, ref itemIndex);

            threshItem.Release();
            threshItem.Dispose();
            gloryItem.Release();
            gloryItem.Dispose();

            return ret;
        }
        public void getNewPicture(HxCard.XImgFrame[] images)
        {
            sw.Start();
            HxCard.XImgFrame image = images[0];
            HxCard.XImgFrame rawImage = images[1];
            int row = (int)image.Height;
            int col = (int)image.Width;
            Mat img = new Mat(new int[] { row, col }, MatType.CV_8UC1, image.Data);
            Mat maskImg = img.Clone();
            Mat rawImg = new Mat(new int[] { row, col }, MatType.CV_8UC1, rawImage.Data);
#if _DETECT
            img.ConvertTo(img, MatType.CV_32F);
            Cv2.Normalize(img, img, 1.0, 0, NormTypes.MinMax);
            img = img * 255;
            img.ConvertTo(img, MatType.CV_8UC1);
            Mat connImage = new Mat(100, 100, MatType.CV_8UC3, new Scalar(0, 0, 0));
            Cv2.Blur(img, img, new OpenCvSharp.Size(3, 3));
            Cv2.Canny(img, img, 10, 80, 3, false); // 输入图像虚为单通道8位图像
            OpenCvSharp.Point[][] contours;
            HierarchyIndex[] hierarchy;
            Cv2.FindContours(img, out contours, out hierarchy, RetrievalModes.External, ContourApproximationModes.ApproxSimple);
#endif
            Mat imgRgb = new Mat(new int[] { row, col }, MatType.CV_8UC3);
            Cv2.CvtColor(rawImg, imgRgb, ColorConversionCodes.GRAY2BGR);

            int i = 0;
            Rect[] boundRect = new Rect[contours.Length];
#if _DETECT
            foreach (OpenCvSharp.Point[] contour in contours)
            {
                double area = Cv2.ContourArea(contours[i]);
                boundRect[i] = Cv2.BoundingRect(contours[i]);                //获取每个contour的框
                if (!(area == 0 || boundRect[i].Height > row / 3))
                {
                    Cv2.DrawContours(imgRgb, contours, i, new Scalar(255, 0, 0), 1);
                    OpenCvSharp.Point point1 = new OpenCvSharp.Point(boundRect[i].BottomRight.X, boundRect[i].BottomRight.Y);
                    OpenCvSharp.Point point2 = new OpenCvSharp.Point(boundRect[i].TopLeft.X, boundRect[i].TopLeft.Y);
                    Cv2.Rectangle(imgRgb, point2, point1, 100, 1);
                }
                Mat mask = maskImg[boundRect[i]];
                Mat tinyItem = rawImg[boundRect[i]];                        //用框裁剪出所有contour的最小mat,生成一个mat[]
                Mat gloryItem = tinyItem.Clone();
                Cv2.BitwiseAnd(tinyItem, mask, gloryItem);
                Scalar sumValue = Cv2.Sum(gloryItem);
                double mean = sumValue.Val0 / area;                         //计算每个mat的平均值
                Cv2.PutText(imgRgb, String.Format("b{0}:{1}", i, mean.ToString("f1")),
                            new OpenCvSharp.Point(boundRect[i].X + 10, boundRect[i].Y),
                            HersheyFonts.HersheySimplex, 0.3, new Scalar(0, 255, 0));
                i++;
            }
#endif
#if _IO
            Bitmap bitmapImg = OpenCvSharp.Extensions.BitmapConverter.ToBitmap(imgRgb);
            DrawImg(bitmapImg);
            string imgPath = ".\\data\\图像5\\";
            if (!Directory.Exists(imgPath))
            {
                Directory.CreateDirectory(imgPath);
            }
            bitmapImg.Save(imgPath + imgIndex.ToString() + ".bmp");
#endif
            imgIndex++;
            sw.Stop();
            Console.WriteLine(sw.ElapsedMilliseconds);
            sw.Reset();
        }
        User user;
        private void FindDeviceButton_Click(object sender, EventArgs e)
        {
            user = new User();
            //DeleteFiles(result_data + "pic/");
            //xsystem = new XSystemW();
            hxCard = new HxCard(512, 100);
            hxCard.ImgPosiontType = HxCard.PositonType.Inverse;
            hxCard.ImgGrayType = HxCard.GrayType.InverseByTexture;
            hxCard.Connect(arrayServer, arrayRemoteIp, arrayLocalRecvPort, arrayRemoteCmdPort, arrayRemoteCmdPort, arrayRemoteDataPort);

            //xsystem.LocalIP = arrayServer;
            //xsystem.OnXError += new XSystemW.DelOnXError(OnError);

            //if (xsystem.Open() > 0)
            //{
            //    int dev_num = xsystem.FindDevice();
            //    if (dev_num > 0)
            //    {
            //        xdevice = xsystem.GetDevice(0);
            //        string s = xdevice.IP;
            //        s = xdevice.CmdPort.ToString();
            //        s = xdevice.ImgPort.ToString();
            //
            //    }
            //}

            try
            {
                Power_Amplifier_Load();
            }
            catch (SocketException ex)
            {
                Error.Text = "Error ID:" + ex.ErrorCode.ToString() + "，功放连接" + ex.Message;
                return;
            }

            //Power_Amplifier_Load();
            //locker = new object();
            open_recv();

            // openclick
            //xdevice = xsystem.GetDevice(0);
            //xfactory = new XGigFactoryW();
            //xcommand = new XCommandW();
            //xcommand.Factory = xfactory;
            //xcommand.OnXError += new XCommandW.DelOnXError(OnError);
            //xcommand.OnXEvent += new XCommandW.DelOnXEvent(OnEvent2);

            if (true)//xcommand.Open(xdevice) > 0)
            {
                //xtransfer = new XFrameTransferW();
                //xtransfer.LineNum = Convert.ToUInt32("512");
                //xtransfer.OnXError += new XFrameTransferW.DelOnXError(OnError);
                //xtransfer.OnXEvent += new XFrameTransferW.DelOnXEvent(OnEvent1);
                //xtransfer.OnXFrameReady += new XFrameTransferW.DelOnFrameReady(OnFrameReady);
                hxCard.RegisterImgCallback(OnFrameReady);
                //hxCard.RegisterImgCallback(getNewPicture);
                //xacquisition = new XAcquisitionW();
                //xacquisition.Factory = xfactory;
                //xacquisition.Transfer = xtransfer;
                //xacquisition.OnXError += new XAcquisitionW.DelOnXError(OnError);
                //xacquisition.OnXEvent += new XAcquisitionW.DelOnXEvent(OnEvent1);
                //xacquisition.EnableLineInfo = 1;
                //MessageBox.Show("Finished Start the Line Info!");

                //if (xacquisition.Open(xdevice, xcommand) > 0)
                //{
                //    xdisplay = new XDisplayW();
                //    xdisplay.Open(xdevice, xtransfer.LineNum, DisWin.Handle, Convert.ToUInt32(0));
                //    xcorrect = new XOffCorrectW();
                //    xcorrect.Open(xdevice);
                //}
                //Get serial number of X-GCU
                //string sn = xcommand.GetPara(51, 0);
                //SN.Text = "SN: " + sn;

                // 连接到PLC

                Connect_to_PLC();
#if _TEST
#else
                //SetIntegralTime();
#endif

                StartButton.Enabled = true;
                FindDeviceButton.Enabled = false;
                FunctionBox.Enabled = true;
            }
        }

        private void RecvMessage()
        {
            while (!quit_flag)
            {
                Msg msg;
                byte[] buf = new byte[10000];
                if (buf == null)
                    throw new Exception();
                int read = 0;
                try
                {
                    read = udpRecv.Receive(buf);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                    if (quit_flag)
                        return;
                }
                msg.getTime = DateTime.Now;

                msg.bytes = buf;
                msg.length = read;
                msg.pickTime = new DateTime(1970, 1, 1, 0, 0, 0, 0);       //这几个变量是现场调试添加
                msg.processTime = new DateTime(1970, 1, 1, 0, 0, 0, 0);
                msg.sendTime = new DateTime(1970, 1, 1, 0, 0, 0, 0);
                msg.beforeDeTime = new DateTime(1970, 1, 1, 0, 0, 0, 0);

                lock (locker)
                {
                    msg.saveTime = DateTime.Now;
                    msg_queue.Enqueue(msg);
                    //label4.Text = msg_queue.Count.ToString();//todo
                }
                conditional_variable?.Set();
            }
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            //wr.WriteLine("Total_num = " + total_clock_num.ToString() + ", Total_detect: " + total_detect_num.ToString());
            //wr.Flush();
            // 暂停线阵
            //wr.Close();
            fs.Close();
            //wr2.Close();
            fs2.Close();
            //AutoCheckTimer.Enabled = false;
            //AutoCheckTimer2.Enabled = false;
            //t.Enabled = false;
            timerthread.Abort();
            recv = false;
            //recv_thread.Abort();
            //xacquisition.Stop();
#if _TEST
#else
            hxCard.Reset();
            xRayPower.Reset();
#endif
            // 暂停功放
            quit_flag = true;
            //udpRecv.Close();
            //listen_thread.Abort();
            lock (locker)
            {
                msg_queue.Clear();
                conditional_variable?.Set();
                //conditional_variable.Set();
                //conditional_variable.Set();
                //conditional_variable.Set();
            }
            //process_thread1.Abort();
            //process_thread2.Join();
            //process_thread3.Join();
            //process_thread4.Join();
            if (writeFlag)
            {
                //write(SCAData);
            }

            ChannelChecktimer.Enabled = false;

            MessageBox.Show("线阵与功放停止运行！");
            StartButton.Enabled = true;
            StopButton.Enabled = false;
            FindDeviceButton.Enabled = true;
            //FunctionBox.Enabled = true;
            paraSetButton.Enabled = true;
            saveData();
        }

        private void ChannelChecktimer_Tick(object sender, EventArgs e)
        {
            //ChannelStateCheck();
        }

        private void paraSetButton_Click(object sender, EventArgs e)
        {
            ParamSettingForm psf = new ParamSettingForm();
            if(psf.ShowDialog()== DialogResult.OK)
            {
                num_of_mouth = psf.nozzleNum;          // 喷嘴数量
                length_belt = psf.beltLength;          // 皮带长度为1000mm
                length_linearray = psf.arrayLength;     // 线阵长度为1200mm
                speed = psf.beltVelocity;              // 传送带速度
                if(xRayPower != null)
                    xRayPower.ResponsThresHold = psf.powerThreshold;
                minItemArea = psf.minItemArea;
                itemGrayThreshold = psf.itemThreshold;
            }
        }

        private void ProcessMessage()
        {
            while (!quit_flag)
            {
                List<Msg> msgs = new List<Msg>();
                conditional_variable?.WaitOne();
                if (quit_flag)
                    return;
                lock (locker)
                {
                    while (msg_queue.Count != 0)
                    {
                        DateTime Detime = DateTime.Now;
                        Msg t = msg_queue.Dequeue();
                        t.beforeDeTime = Detime;
                        t.pickTime = DateTime.Now;
                        msgs.Add(t);
                    }
                }
                foreach (Msg msg in msgs)
                {
                    ProcessMsg(msg);
                }
            }
        }

        private void ProcessMsg(Msg msg)
        {
            string s = bytetohexstr(msg.bytes, msg.length);
            string rawdata = s.Replace(" ", "");     //视情况可删减
            dataProcess dp = new dataProcess();

            //label1.Text = dp.processway.ToString();//todo

            int index = 0;
            Interlocked.Increment(ref CycleCount);
            while (index < rawdata.Length - 6)        //数据处理，测试处理时间基本在6-7ms
            {
                int SectionLen = 0;
                int SectionType = 0;

                SectionLen = dp.GetN(rawdata, ref index, 2);
                SectionType = dp.GetN(rawdata, ref index, 1);

                switch (SectionType)
                {
                    case 1:
                        recvData = dp.ChannelMetaSection(ref index, SectionLen, rawdata);
                        chnldx = recvData[0];
                        for (int temp = 1; temp < 7; temp++)
                        {
                            channelStateData[chnldx - 1] += recvData[temp];
                        }
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
                msg.processTime = DateTime.Now;
                Interlocked.Increment(ref intocount);
                //label3.Text = intocount.ToString();

                /*
                //调试用保存数据
                if (writeFlag)
                {
                    int j = 0;
                    for (int i = 0; i < 3; i++)
                    {
                        SCAData[CycleCount / 10, chnldx - 1, j++] = dp.SCAStartBin[i];
                        SCAData[CycleCount / 10, chnldx - 1, j++] = dp.SCALength[i];
                        SCAData[CycleCount / 10, chnldx - 1, j++] = dp.SCACount[i];
                    }
                    SCAData[CycleCount / 10, chnldx - 1, 9] = chnldx;
                    DataGetTime[CycleCount / 10, chnldx - 1] = msg.getTime.TimeOfDay.ToString();
                    processTime[CycleCount / 10, chnldx - 1] = msg.processTime.TimeOfDay.ToString();
                    saveTime[CycleCount / 10, chnldx - 1] = msg.saveTime.TimeOfDay.ToString();
                    pickTime[CycleCount / 10, chnldx - 1] = msg.pickTime.TimeOfDay.ToString();
                    beforeDeTime[CycleCount / 10, chnldx - 1] = msg.beforeDeTime.TimeOfDay.ToString();

                }
                */

                //if(FunctionSelect_Cu.Checked && dp.SCACount[0] > gap)
                if ((FunctionSelect_Cu.Checked && dp.SCACount[1] > 25) || 
                    (FunctionSelect_Zn.Checked && dp.SCACount[0] > 25) || 
                    (FunctionSelect_Pb.Checked && (dp.SCACount[3] > gap_pb || dp.SCACount[2] > gap_pb)) || 
                    (FunctionSelect_Fe.Checked && dp.SCACount[4] > 30) ||
                    (FunctionSelect_YelCu.Checked && dp.SCACount[1] > 15 && dp.SCACount[0] > 15))           //符合条件的数据报，发送给HJ
                {
                    //sendcount++;
                    //label4.Text = sendcount.ToString();
                    byte[] gfData = new byte[10];
                    TimeSpan timeStamp = msg.getTime - new DateTime(1970, 1, 1, 0, 0, 0, 0);
                    long temp = (Int64)timeStamp.TotalMilliseconds - 40;
                    for (int i = 0; i < 8; i++)
                    {
                        gfData[i] = (byte)(temp >> (i * 8) & 0xff);
                    }
                    byte[] c = new byte[2];
                    c = BitConverter.GetBytes(chnldx);
                    gfData[8] = c[0];
                    gfData[9] = c[1];
                    udpSend.Send(gfData);
                    msg.sendTime = DateTime.Now;
                    //sendTime[CycleCount / 10, chnldx - 1] = msg.sendTime.TimeOfDay.ToString();
                    Console.WriteLine("有物块");
                }
                else
                {
                    Console.WriteLine("无物块");
                }
            }
            else
            {
                MessageBox.Show("channel index error!");
            }
        }

        public string bytetohexstr(byte[] bytes, int length)     //字节转化为二进制
        {
            if (bytes == null)
                return "";
            char[] r = new char[2 * length];
            for (int i = 0; i < length; i++)
            {
                int high = bytes[i] / 16;
                int low = bytes[i] % 16;
                r[i * 2] = idx_to_hex[high];
                r[i * 2 + 1] = idx_to_hex[low];
            }
            string ret = new string(r);
            return ret;
        }

        /*
        public void write(int[,,] arr)             //保存txt文本
        {
            string temppath = @"D:\422test\data";


            for (int k = 0; k < CycleCount / 10; k++)
            {
                for (int i = 0; i < 10; ++i)
                {
                    //if (arr[k, i, 2] > gap || arr[k, i, 5] > gap || arr[k, i, 8] > gap)
                    if (arr[k, i, 2] > gap)
                    {
                        int temp = i + 1;
                        string txtname = temppath + k + "." + temp;
                        txtname = txtname + ".txt";
                        FileStream fs = new FileStream(txtname, FileMode.Create);
                        StreamWriter sw = new StreamWriter(fs);
                        sw.WriteLine(i);
                        sw.WriteLine(DataGetTime[k, i]);
                        sw.WriteLine(saveTime[k, i]);
                        sw.WriteLine(beforeDeTime[k, i]);
                        sw.WriteLine(pickTime[k, i]);
                        sw.WriteLine(processTime[k, i]);
                        sw.WriteLine(sendTime[k, i]);


                        for (int j = 0; j < 10; ++j)
                        {
                            sw.Write(arr[k, i, j] + " ");
                        }
                        sw.Flush();
                        //关闭流
                        sw.Close();
                        fs.Close();

                    }

                }
            }
            MessageBox.Show("write finish!");
        }
        */
        private void DrawImg(Bitmap img)
        {
            Rectangle src = new Rectangle(0, 0, img.Width, img.Height);
            Rectangle dst = new Rectangle(0, 0, DisWin.Width, DisWin.Height);
            DisWin.CreateGraphics().DrawImage(img, dst, src, GraphicsUnit.Pixel);
        }
        private void DrawImg(HxCard.XImgFrame img)
        {
            Rectangle src = new Rectangle(0, 0, (int)img.Width, (int)img.Height);
            Rectangle dst = new Rectangle(0, 0, DisWin.Width, DisWin.Height);
            Console.WriteLine(img.Data.Length);
            Bitmap dataImg = ByteToGrayBitmap(img.Data, (int)img.Width, (int)img.Height);
            DisWin.CreateGraphics().DrawImage(dataImg, dst, src, GraphicsUnit.Pixel);
            //dataImg.Save(".\\data\\图像2\\" + imgIndex.ToString() + ".bmp");
        }
        public static Bitmap ByteToGrayBitmap(byte[] rawValues, int width, int height)
        {
            //申请目标位图的变量，并将其内存区域锁定
            Bitmap bmp = new Bitmap(width, height, PixelFormat.Format8bppIndexed);
            BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height),
             ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);

            //获取图像参数
            int stride = bmpData.Stride;  // 扫描线的宽度  
            int offset = stride - width;  // 显示宽度与扫描线宽度的间隙  
            IntPtr iptr = bmpData.Scan0;  // 获取bmpData的内存起始位置  
            int scanBytes = stride * height;// 用stride宽度，表示这是内存区域的大小  

            //下面把原始的显示大小字节数组转换为内存中实际存放的字节数组
            int posScan = 0, posReal = 0;// 分别设置两个位置指针，指向源数组和目标数组  
            byte[] pixelValues = new byte[scanBytes];  //为目标数组分配内存  

            for (int x = 0; x < height; x++)
            {
                //下面的循环节是模拟行扫描
                for (int y = 0; y < width; y++)
                {
                    pixelValues[posScan++] = rawValues[posReal++];
                }
                posScan += offset;  //行扫描结束，要将目标位置指针移过那段“间隙”  
            }

            //用Marshal的Copy方法，将刚才得到的内存字节数组复制到BitmapData中
            System.Runtime.InteropServices.Marshal.Copy(pixelValues, 0, iptr, scanBytes);
            //解锁内存区域  
            bmp.UnlockBits(bmpData);

            //下面的代码是为了修改生成位图的索引表，从伪彩修改为灰度
            ColorPalette tempPalette;
            using (Bitmap tempBmp = new Bitmap(1, 1, PixelFormat.Format8bppIndexed))
            {
                tempPalette = tempBmp.Palette;
            }
            for (int i = 0; i < 256; i++)
            {
                tempPalette.Entries[i] = System.Drawing.Color.FromArgb(i, i, i);
            }
            bmp.Palette = tempPalette;
            return bmp;
        }

        public int TimeConvert2Index(long imgTime,long itemTime)
        {
            long timeLength = itemTime - imgTime;
            return (int)(timeLength / integral_time) + hxCard.ImgHeight;
        }

        private void refreshThreshButton_Click(object sender, EventArgs e)
        {
            hxCard.RefreshThresh();
        }

        private void TestForm_FormClosed(object sender, FormClosedEventArgs e)
        {

        }

        private void TestForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            //wr.WriteLine("Total_num = " + total_clock_num.ToString() + ", Total_detect: " + total_detect_num.ToString());
            //wr.Flush();
            // 暂停线阵
            //wr.Close();
            fs?.Close();
            //wr2.Close();
            fs2?.Close();
            //AutoCheckTimer.Enabled = false;
            //AutoCheckTimer2.Enabled = false;
            //t.Enabled = false;
            timerthread?.Abort();
            recv = false;
            //recv_thread.Abort();
            //xacquisition.Stop();
#if _TEST
#else
            hxCard?.Reset();
#endif
            // 暂停功放
            quit_flag = true;
            //udpRecv.Close();
            //listen_thread?.Abort();
            lock (locker)
            {
                msg_queue?.Clear();
                conditional_variable?.Set();
                //conditional_variable.Set();
                //conditional_variable.Set();
                //conditional_variable.Set();
            }
            //process_thread1?.Abort();
            //process_thread2.Join();
            //process_thread3.Join();
            //process_thread4.Join();
            if (writeFlag)
            {
                //write(SCAData);
            }

            ChannelChecktimer.Enabled = false;

            MessageBox.Show("线阵与功放停止运行！");
            StartButton.Enabled = true;
            StopButton.Enabled = false;
            FindDeviceButton.Enabled = true;
            //FunctionBox.Enabled = true;
            paraSetButton.Enabled = true;
            saveData();
        }

        public float LineConvert2Belt(float x)
        {
            float ret = 0;
            if (x < p1) 
            {
                ret = t1 - (p1 - x) * SOD / SDD;
            }
            else
            {
                ret = (x - p1) * SOD / SDD + t1;
            }
            return ret;
        }

        public float BeltConvert2Line(float x)
        {
            float ret = 0;
            if (x < p1)
            {
                ret = p1 - (t1 - x) * SDD / SOD;
            }
            else
            {
                ret = (x - t1) * SDD / SOD + p1;
            }
            return ret;
        }
        public short PixelConvert2Num(float beltPixel)
        {
            return (Int16)(num_of_mouth * beltPixel / (t1 + t2));
        }
        //int minItemArea = 20;
        //int itemGrayThreshold = 80;
        //int subItemGrayThresh = 60;
        //int subItemAreaThresh = 20;
        private void selfDefParam()
        {
            string[] content = File.ReadAllLines(".\\测试1\\参数.txt");
            minItemArea = Convert.ToInt32(content[5]);
            itemGrayThreshold = Convert.ToInt32(content[6]);
            subItemGrayThresh = Convert.ToInt32(content[7]);
            subItemAreaThresh = Convert.ToInt32(content[8]);
            grayMeanDiffThresh = Convert.ToInt32(content[9]);
            Console.WriteLine("minItemArea:{0},itemGrayThreshold:{1},subItemGrayThresh:{2},subItemAreaThresh:{3},grayMeanDiffThresh:{4}"
                , minItemArea, itemGrayThreshold, subItemGrayThresh, subItemAreaThresh, grayMeanDiffThresh);
        }
        private void saveData()
        {
            string result = string.Format("Date:{0}\ntotal item num:{1}\nchoosed item num:{2}\nchoosed persentage:{3}", DateTime.Now, trueItemCunt, choosedItemCunt, ((float)choosedItemCunt / trueItemCunt * 100));
            File.WriteAllText(@".\测试1\result.txt", result);
            string sendedDataMsgStr = "send_Num:" + check.ToString() + "\n";
            sendedDataMsgStr += "red_Num:" + choosedItemCunt + "\n";
            foreach(var data in sendedMsgList)
            {
                sendedDataMsgStr += string.Format("syn_code:{0}\nflow_num:{1}\ntypof_block:{2}\nblow:{3}\n" +
                    "start_time_int:{4}\nmillionseconds{5}\nblow_time:{6}\nstart_num:{7}\n" +
                    "end_num:{8}\nreserve:{9}\ncheck_num:{10}\n===========================\n", data.syn_code, data.flow_num, data.typof_block, data.blow,
                    data.start_time_int, data.millionseconds, data.blow_time, data.start_num, data.end_num, data.reserve, data.check_num);
            }
            File.WriteAllText(@".\测试1\sendMessages.txt", sendedDataMsgStr);
        }
    }


}


