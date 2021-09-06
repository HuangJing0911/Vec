/*
=======================================================================================
============================最新版本20210415==========================================
0415_解决了读取不到时间戳的问题，目前还需要对时间戳信息进行分析
=======================================================================================
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using XLibWrapper;
using System.Threading;
using OpenCvSharp;
using System.Net.Sockets;
using System.Net;
using HslCommunication;
using HslCommunication.Profinet.Omron;
using Newtonsoft.Json;

namespace XEthernetDemo
{
    struct Msg
    {
        internal byte[] bytes;
        internal int length;
        internal DateTime getTime;
        internal DateTime saveTime;
        internal DateTime pickTime;
        internal DateTime processTime;
        internal DateTime sendTime;
        internal DateTime beforeDeTime;
    }

    struct GFinfo
    {
        internal int channelindex;          // 物块在第几通道
        internal Int64 time;             // 物块到达的时间
        internal int flag;                  // 物块的类型
    }



    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            PsColor.SelectedIndex = 0;
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(CurrentDomain_UnhandleException);
        }

        public void SetSetting()
        {
            LocalAdapter.Text = this.ntpServer;
            Total_Block_Num.Text = "Speed = " + speed;
        }

        private static void CurrentDomain_UnhandleException(object sender, UnhandledExceptionEventArgs e)
        {
            Console.WriteLine(e.ExceptionObject.ToString());
            MessageBox.Show(e.ExceptionObject.ToString());
        }



        //功放变量
        static Queue<Msg> msg_queue;
        static Queue<GFinfo> info_queue;
        static Socket socket;
        static AutoResetEvent conditional_variable;
        static Object gflocker;
        static volatile bool quit_flag = false;
        static IPEndPoint ep;

        static char[] idx_to_hex = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

        public int[,,] SCAData = new int[10000, 10, 10];    //40为一秒轮次，可存储25秒数据
        string[,] DataGetTime = new string[10000, 10];
        string[,] processTime = new string[10000, 10];
        string[,] sendTime = new string[10000, 10];
        string[,] saveTime = new string[10000, 10];
        string[,] pickTime = new string[10000, 10];
        string[,] beforeDeTime = new string[10000, 10];

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
        public int gap = 15;        //阈值

        private Thread listen_thread;
        private Thread process_thread1, process_thread2, process_thread3, process_thread4;

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
        XSystemW xsystem;
        XDeviceW xdevice;
        XGigFactoryW xfactory;
        XCommandW xcommand;
        XFrameTransferW xtransfer;
        XAcquisitionW xacquisition;
        XDisplayW xdisplay;
        XOffCorrectW xcorrect;
        XTifFormatW xtifform;

        int frame_count = 0;
        int lost_line = 0;
        int total_card_num = 0;
        uint cur_dm_index = 0;
        int total_clock_num = 0;
        Int64 time_now;
        Int64 time_finish;
        int pic_num = 0;
        int check_time_num = 0;         // 定时器1的计数器
        int check_time_num2 = 0;        // 定时器2的计数器
        float integral_time = 3;        // 默认扫描积分时间3ms
        public float speed = 3.0f;              // 皮带速度定为3.0
        public int num_of_mouth = 198;          // 喷嘴数量
        public int length_belt = 1016;          // 皮带长度为1000mm
        public int length_linearray = 1120;     // 线阵长度为1200mm
        uint time_interval = 4;                 // 定时器定时时间为5ms
        public float SDD = 914;
        public float SOD = 815;
        string result_pic;
        string init_pic;
        Socket client = null;       //与PLC连接的socket
        Socket client2 = null;      //与功放程序连接的socket
        public byte[] Rcvbuffer;    //接收字节组数
        delegate void AppendDelegate(string str);
        AppendDelegate AppendString;
        string test_txt_filepath = "C:/Users/weike/Desktop/TEST17.txt";
        string result_data = "C:/Users/weike/Desktop/0413_data/result-data.txt";
        string time_data = "C:/Users/weike/Desktop/0413_data/frame-time-data.txt";
        FileStream fs;
        StreamWriter wr;
        FileStream fs2;
        StreamWriter wr2;
        public string ntpServer;      // 线阵IP
        public string ntpServer2 = "127.0.0.1";
        public string PowerIP = "172.28.110.50";        // 功放IP
        OmronFinsNet omronFinisNet = new OmronFinsNet("192.168.250.1", 6001);
        //System.Timers.Timer t = new System.Timers.Timer(5);
        Thread timerthread;
        Thread recv_thread;
        static Object locker;
        GFinfo first_info = new GFinfo();

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
                if (client2.Connected)
                    Total_Block_Num.Text = "Successfully Connect!";
                else
                    Total_Block_Num.Text = "Unsuccessfully Connect!";
                //client2.ReceiveTimeout = 3000;
            }
        }

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
                info.time = Convert.ToInt64(time.ToString());
                //index = index << 8 & data[9];
                info.channelindex = Convert.ToInt32(data[8].ToString("X2"), 16);
                info.flag = 1;      // 入队列的为铜的信息
                wr.WriteLine("Receive time: " + info.time.ToString() + ",index: " + info.channelindex.ToString());
                wr.Flush();
                //lock(locker)
                //{
                info_queue.Enqueue(info);
                //}

            }
            if (!recv)
            {
                Total_Block_Num.Text = "Stop Receive!";
                Thread.CurrentThread.Abort();
            }

        }


        // 测试代码
        Mat GetTif_as_mat(string filepath)  //将tif转为mat
        {
            Mat image = Cv2.ImRead(filepath);
            float value = image.At<float>(0, 1);
            return image;
        }

        Mat GetTXT_as_mat(string filepath)  //将txt转为mat
        {
            StreamReader rd = File.OpenText(filepath);
            string[] str = File.ReadAllLines(filepath);
            string[] ss = str[0].Split('\t');
            int row = str.Length;       //行数
            int col = ss.Length - 1;    //列数
            col = col / 2 - 2;

            int[,] p1 = new int[row, col]; //数组
            int maxp = 0;
            int minp = 65536;

            for (int i = 0; i < row; i++)  //读入数据并赋予数组
            {
                string line = rd.ReadLine();
                string[] data = line.Split('\t');
                for (int j = 0; j < col; j++)
                {
                    p1[i, j] = int.Parse(data[j + 2]);
                    if (p1[i, j] > maxp)
                        maxp = p1[i, j];
                    else if (p1[i, j] < minp)
                        minp = p1[i, j];
                }
            }

            // 把图像归一化
            //读入数据并赋予数组
            int m, n;
            Mat image = new Mat(row, col, MatType.CV_8UC1);
            for (m = 0; m < row; m++)
            {
                for (n = 0; n < col; n++)
                {
                    float k = ((float)(p1[m, n] - minp)) / (maxp - minp);
                    k = k * 255;
                    p1[m, n] = (int)k;
                    image.Set(m, n, p1[m, n]);
                }
            }
            //image.ConvertTo(image, MatType.CV_32F);
            //float value2 = image.At<float>(0, 0);
            //Cv2.Normalize(image, 1image, 1.0, 0, NormTypes.MinMax);
            //value2 = image.At<float>(0, 0);
            //int value = image.At<int>(0, 0);
            //image = (image - minp) / (maxp - minp);
            //value = image.At<int>(0, 0);
            //image = image * 255;
            image.ConvertTo(image, MatType.CV_8UC3);
            CardNum1.Text = "Height = " + Convert.ToString(image.Height);
            CardNum2.Text = "Width = " + Convert.ToString(image.Width);
            Cv2.ImWrite("C:/Users/weike/Desktop/TEST_PRE.png", image);
            return image;
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

        private void ConnectCallback(IAsyncResult ar)
        {
            try
            {

                //Socket client1 = (Socket)ar.AsyncState;
                client.EndConnect(ar);
                CardNum1.Invoke(AppendString, String.Format("已经成功连接到服务器{0}！", client.RemoteEndPoint.ToString()));
                CardNum2.Invoke(AppendString, String.Format("本地端接点为{0}！", client.LocalEndPoint.ToString()));
                Rcvbuffer = new byte[client.SendBufferSize];
                //AsyncCallback callback = new AsyncCallback(ReceiveCallback);
                //client.BeginReceive(Rcvbuffer, 0, Rcvbuffer.Length, SocketFlags.None, callback, client);
            }
            catch
            {
                CardNum1.Invoke(AppendString, String.Format("无法建立与服务器的连接！"));
            }
        }

        void OnFrameReady_testimage()
        {
            //测试数据
            time_now = DateTime.Now.Millisecond;
            Mat image = GetTXT_as_mat(test_txt_filepath);
            Mat connImage = new Mat(100, 100, MatType.CV_8UC3, new Scalar(0, 0, 0));
            CardNum1.Text = Convert.ToString(image.Height);
            CardNum2.Text = Convert.ToString(image.Width);
            image.CopyTo(connImage);

            Cv2.Blur(image, image, new OpenCvSharp.Size(3, 3));
            //Cv2.CvtColor(image, image, ColorConversionCodes.BGR2GRAY); // 具体看输入图像为几通道，单通道则注释
            Cv2.Canny(image, image, 10, 80, 3, false); // 输入图像虚为单通道8位图像

            OpenCvSharp.Point[][] contours;
            HierarchyIndex[] hierarchy;
            Cv2.FindContours(image, out contours, out hierarchy, RetrievalModes.External, ContourApproximationModes.ApproxSimple);

            Rect[] boundRect = new Rect[contours.Length];
            RotatedRect[] box = new RotatedRect[contours.Length];
            //List<OpenCvSharp.Point[]> Resultcontours = new List<OpenCvSharp.Point[]>();


            Console.WriteLine("=====================");
            Console.WriteLine(contours.Length);
            Console.WriteLine("=====================");
            time_finish = DateTime.Now.Millisecond;
            CardNum3.Text = Convert.ToString(time_finish - time_now) + "ms";
            Console.WriteLine("process picture spend {0} millisecond", time_finish - time_now);
            if (contours.Length != 0)
            {
                Console.WriteLine("=====================");
                Console.WriteLine("successfully detect{0} blocks!", contours.Length);
                Console.WriteLine("=====================");
                total_card_num += contours.Length;
                Total_Block_Num.Text = Convert.ToString(total_card_num);
                /*  测试轮廓位置
                for (int i = 0; i < contours.Length; i++)
                {
                    Console.WriteLine("=====================================");
                    for (int j = 0; j < contours[i].Length; j++)
                        Console.WriteLine("contour[{0}][{1}] value:{2},{3}", i, j, contours[i][j].X, contours[i][j].Y);
                }
                Console.WriteLine("=====================================");
                */
            }
            //Total_Block_Num.Text = Convert.ToString(total_card_num);
            // 矩形轮廓绘画
            for (int i = 0; i < contours.Length; i++)
            {
                double area = Cv2.ContourArea(contours[i]);
                if (area == 0) continue;
                Scalar color = new Scalar(0, 0, 255);
                //Cv2.DrawContours(connImage, contours, i, color, 1, LineTypes.Link8, hierarchy);
                boundRect[i] = Cv2.BoundingRect(contours[i]);
                Cv2.Rectangle(connImage, new OpenCvSharp.Point(boundRect[i].X, boundRect[i].Y),
                    new OpenCvSharp.Point(boundRect[i].X + boundRect[i].Width, boundRect[i].Y + boundRect[i].Height),
                    new Scalar(0, 255, 0), 1, LineTypes.Link8);
            }
            Cv2.ImWrite("C:/Users/weike/Desktop/TEST_result.bmp", connImage);
            /*
            XImageW imageW ;
            int[,] matrix = new int[row, col];
            for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                    matrix[i,j] = image.Get<int>(i, j);
            for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                    matrix[i, j] = image.Get<int>(i, j);
            */
            image.Dispose();
            connImage.Dispose();
        }
        void OnFrameReady(XImageW image)
        //void OnFrameReady(XTifFormatW image)
        {
            //string x = xtifform.GetPara(12);
            frame_count++;
            /*
            Thread thread = new Thread(new ParameterizedThreadStart(threadCounters));
            thread.Start(image);
            thread.Join();
            */
            threadCounters(image);
            xdisplay.Display(image);
        }

        int count_thread = 0;
        void threadCounters(Object obj)
        {
            DateTime stamp = DateTime.Now;
            XImageW image = (XImageW)obj;
            //XTifFormatW image_tif = new XTifFormatW(image,xdevice);
            pic_num++;
            string save_file;
            save_file = "C:/Users/weike/Desktop/0413_data/2_with_timestamp/TEST" + pic_num + ".txt";
            //string save_tif = "C:/Users/77170/Desktop/0316/pic_data/TEST" + pic_num + ".tif";
            //image.Save(save_file);
            //image_tif.Save(save_tif);
            int value = Marshal.ReadInt32(image.DataAddr, 0);

            try
            {
                //Console.WriteLine("thread" + count_thread);
                //XImageW image = (XImageW)obj;
                time_now = DateTime.Now.Millisecond;
                uint[,] pixelval = new uint[image.Height, image.Width];
                uint maxp = 0;
                uint minp = 65536;
                int m, n, row, col;
                row = (int)image.Height;
                col = (int)image.Width;
                col = col / 2 - 2;
                for (uint i = 0; i < row; i++)
                    for (uint j = 0; j < col; j++)
                    {
                        pixelval[i, j] = (uint)image.GetPixelVal(i, j + 2);
                        if (pixelval[i, j] > maxp)
                            maxp = pixelval[i, j];
                        else if (pixelval[i, j] < minp)
                            minp = pixelval[i, j];
                    }

                Mat image_mat = new Mat(row, col, MatType.CV_8UC1);
                for (m = 0; m < row; m++)
                {
                    for (n = 0; n < col; n++)
                    {
                        float k = ((float)(pixelval[m, n] - minp)) / (maxp - minp);
                        k = k * 255;
                        image_mat.Set(m, n, (int)k);
                    }
                }
                //Console.WriteLine("successfully get array");
                time_finish = DateTime.Now.Millisecond;
                Console.WriteLine("==================================");
                Console.WriteLine("read pixel value spend {0} millisecond", time_finish - time_now);
                getCounters_Pixel(image, image_mat, row, col, MatType.CV_16UC1, stamp);
                //Thread.Sleep(1000);
                //Console.WriteLine("thread" + (count_thread++) + " done");
            }
            catch (ThreadAbortException e)
            {
                MessageBox.Show(e.Message);
            }

        }

        // 时间戳对比测试函数
        public void write_stamp(ushort[,] stamp)
        {
            string filename = "C:/Users/weike/Desktop/0413_data/test.txt";
            string sign = "\t";
            StreamWriter sw = new StreamWriter(filename, true);
            for (int i = 0; i < 512; i++)
            {
                for (int j = 0; j < 896; j++)
                    sw.Write(stamp[i, j] + sign); //如果不是string数组，可使用.Tostring()转换在进行连接
                sw.WriteLine();
            }
            sw.Flush();
            sw.Close();
            sw.Dispose();
        }

        // 获取时间戳函数(第一版暂时先获得整个图像数据的时间戳)
        public ushort[,] get_timestamp(XImageW image)
        {
            ushort[,] line_info = new ushort[(int)image.Height, (int)image.Width];
            uint dep = image.PixelDepth;

            if (dep == 16)
            {
                uint stride = image.Width + image.DataOffset;
                unsafe
                {
                    char* pImageAddr = (char*)image.DataAddr;
                    for (int hi = 0; hi < image.Height; hi++)
                    {
                        ushort* pLineAddr = (ushort*)(pImageAddr + stride);
                        for (int wi = 0; wi < image.Width; wi++)
                        {
                            // *(pLineAddr + wi) = (ushort)(wi);
                            line_info[hi, wi] = *(pLineAddr + wi);
                            // Console.WriteLine(*(pLineAddr + wi));
                        }
                    }
                }

            }
            /*
            byte[,] line_info = new byte[(int)image.Height, (int)image.DataOffset];
            if (dep == 16)
            {
                // uint stride = image.Width + image.DataOffset;
                unsafe
                {
                    byte* pImageAddr = (byte*)image.DataAddr;
                    for (int hi = 0; hi < image.Height; hi++)
                    {
                        byte* pLineAddr = (byte*)(pImageAddr + image.Width);
                        for (int wi = 0; wi < image.DataOffset; wi++)
                        {
                            // *(pLineAddr + wi) = (ushort)(wi);
                            line_info[hi, wi] = *(pLineAddr + wi);
                            // Console.WriteLine(*(pLineAddr + wi));
                        }
                    }
                }

            }*/
            write_stamp(line_info);
            return line_info;
        }


        public ushort[,] get_timestamp_test(XImageW image)
        {
            ushort[,] line_info = new ushort[(int)image.Height, (int)(image.Width + image.DataOffset / 2)];
            uint dep = image.PixelDepth;
            if (dep == 16)
            {
                uint stride = image.Width + image.DataOffset / 2;
                unsafe
                {
                    char* pImageAddr = (char*)image.DataAddr;
                    ushort* pLineAddr = (ushort*)pImageAddr;
                    for (int hi = 0; hi < image.Height; hi++)
                    {
                        byte* time_info = (byte*)pLineAddr;
                        for (int wi = 0; wi < (image.Width + image.DataOffset / 2); wi++)
                        {
                            line_info[hi, wi] = *(pLineAddr + wi);
                        }
                        pLineAddr = (ushort*)(pLineAddr + stride);
                    }
                }
            }
            write_stamp(line_info);
            return line_info;
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
                if (client.Connected)
                    Total_Block_Num.Text = "Successfully Connect!";
                else
                    Total_Block_Num.Text = "Unsuccessfully Connect!";
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
                Console.WriteLine("blow time is:" + ntp_data[15] + "  " + ntp_data[16]);
                c = BitConverter.GetBytes(data.start_num);
                ntp_data[17] = c[0];
                ntp_data[18] = c[1];                        // 开始吹气阀号
                c = BitConverter.GetBytes(data.end_num);
                ntp_data[19] = c[0];
                ntp_data[20] = c[1];                        // 开始吹气阀号
                ntp_data[21] = data.reserve;                // 保留值
                ntp_data[22] = data.check_num;              // 校验值

                client.ReceiveTimeout = 3000;
                return client.Send(ntp_data);
            }
        }

        // 发送当前时间点的时间戳
        public void timecheck()
        {
            DateTime stamp = DateTime.Now;
            Data_Set data = new Data_Set();
            data.check_num = 1;     // 此时数据包为检验数据包，进行时间同步
            check_time_num++;

            /*
            data.start_time[0] = (byte)(stamp.Year - 2000);
            data.start_time[1] = (byte)(stamp.Month);
            data.start_time[2] = (byte)(stamp.Day);
            data.start_time[3] = (byte)(stamp.Hour);
            data.start_time[4] = (byte)(stamp.Minute);
            data.start_time[5] = (byte)(stamp.Second);
            data.millionseconds = (Int16)stamp.Millisecond;
            */

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
                wr.WriteLine("Check!" + '\t' + Convert.ToString(data.start_time_int) + '\t');
                wr.Flush();
            }
            // wr.WriteLine("Check!" + '\t' + Convert.ToString(data.start_num) + '\t' + Convert.ToString(data.end_num) + '\t' + "20" + data.start_time[0] + "-" + data.start_time[1]+ "-" + data.start_time[2] + " " + data.start_time[3] +":" + data.start_time[4] + ":" + data.start_time[5] + ":" + data.millionseconds + "ms" +  '\t' + data.blow_time + "ms");
            // wr.Flush();
            // DateTime stamp2 = DateTime.Now;
            // CardNum5.Text = Convert.ToString(stamp2.Millisecond - stamp.Millisecond);
        }

        // 获得格林威治时间函数
        private uint timeGetTime()
        {
            TimeSpan time_stamp = DateTime.Now - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return (uint)time_stamp.TotalMilliseconds;
        }


        // 线程处理函数
        public void thread_for_sending(object obj)
        {
            wr.WriteLine(Thread.CurrentThread.ManagedThreadId + " thread start!");
            Data_Set data = (Data_Set)obj;
            DateTime stamp = DateTime.Now;

            // 计算休眠时间
            int sleepfor = 0;   // 当前时间到检测到物块时间的差值
            if (stamp.Second > data.start_time[5])
            {
                sleepfor = 1000 - data.start_time[5] + stamp.Millisecond;
            }
            else if (stamp.Second == data.start_time[5])
            {
                sleepfor = stamp.Millisecond - data.start_time[5];
            }
            else
            {
                sleepfor = 0;
            }
            wr.WriteLine(Convert.ToString(frame_count) + '\t' + Convert.ToString(data.start_num) + '\t' + Convert.ToString(data.end_num) + '\t' + "20" + data.start_time[0] + "-" + data.start_time[1] + "-" + data.start_time[2] + " " + data.start_time[3] + ":" + data.start_time[4] + ":" + data.start_time[5] + ":" + data.millionseconds + "ms" + '\t' + data.blow_time + "ms\t" + sleepfor);


            // 线程休眠
            int delay_time = (int)(2400 / speed - sleepfor);    // 需要延迟的时间
            int check = 50;
            if (delay_time - check > 0 && sleepfor != 0)
                Thread.Sleep(delay_time - check); // 线程休眠一段时间

            int num = SendData(data);
            if (num == 23)
            {
                total_clock_num++;
                CardNum1.Text = Convert.ToString(total_clock_num) + " blocks is seccessfully send!";
                CardNum2.Text = "start:" + Convert.ToString(data.start_num) + "," + "end:" + Convert.ToString(data.end_num);
                CardNum3.Text = Convert.ToString(data.start_time[4]) + ":" + Convert.ToString(data.start_time[5]) + "s" + Convert.ToString(data.millionseconds) + "ms";
                CardNum4.Text = Convert.ToString(DateTime.Now.Millisecond - stamp.Millisecond) + "ms";
            }

        }

        public void getCounters_Pixel(XImageW ximagew, Mat image, int row, int col, MatType type, DateTime stamp)
        {
            //CardNum2.Text = Convert.ToString(row)+"row";
            //CardNum3.Text = Convert.ToString(col) + "col";
            time_now = DateTime.Now.Millisecond;
            //Mat image = new Mat(row, col, type,p);
            image.ConvertTo(image, MatType.CV_32F);
            Cv2.Normalize(image, image, 1.0, 0, NormTypes.MinMax);
            image = image * 255;
            image.ConvertTo(image, MatType.CV_8UC1);
            init_pic = "C:/Users/weike/Desktop/0413_data/2_with_timestamp/init" + pic_num + ".png";
            //Cv2.ImWrite(init_pic, image);
            Mat connImage = new Mat(100, 100, MatType.CV_8UC3, new Scalar(0, 0, 0));
            image.CopyTo(connImage);
            Cv2.Blur(image, image, new OpenCvSharp.Size(3, 3));
            //Cv2.CvtColor(image, image, ColorConversionCodes.BGR2GRAY); // 具体看输入图像为几通道，单通道则注释
            Cv2.Canny(image, image, 10, 80, 3, false); // 输入图像虚为单通道8位图像


            OpenCvSharp.Point[][] contours;
            HierarchyIndex[] hierarchy;
            Cv2.FindContours(image, out contours, out hierarchy, RetrievalModes.External, ContourApproximationModes.ApproxSimple);

            Rect[] boundRect = new Rect[contours.Length];
            RotatedRect[] box = new RotatedRect[contours.Length];
            //List<OpenCvSharp.Point[]> Resultcontours = new List<OpenCvSharp.Point[]>();


            //Console.WriteLine("=====================");
            //Console.WriteLine(contours.Length);
            //Console.WriteLine("=====================");
            time_finish = DateTime.Now.Millisecond;
            Console.WriteLine("process picture spend {0} millisecond", time_finish - time_now);



            if (contours.Length != 0)
            {
                //Console.WriteLine("=====================");
                Console.WriteLine("successfully detect{0} blocks!", contours.Length);
                Console.WriteLine("==================================\n\n");
                total_card_num += contours.Length;
                // Total_Block_Num.Text = Convert.ToString(total_card_num);
                // 画出检测的轮廓
                int flag = 0;
                for (int i = 0; i < contours.Length; i++)
                {
                    if (contours.Length > 50)
                    {
                        break;
                    }

                    double area = Cv2.ContourArea(contours[i]);
                    boundRect[i] = Cv2.BoundingRect(contours[i]);
                    if (area == 0 || boundRect[i].Height > row / 3)
                        continue;
                    Scalar color = new Scalar(0, 0, 255);
                    Cv2.DrawContours(connImage, contours, i, color, 2, LineTypes.Link8, hierarchy);
                    Cv2.Rectangle(connImage, new OpenCvSharp.Point(boundRect[i].X, boundRect[i].Y),
                        new OpenCvSharp.Point(boundRect[i].X + boundRect[i].Width, boundRect[i].Y + boundRect[i].Height),
                        new Scalar(0, 255, 0), 2, LineTypes.Link8);
                }
                result_pic = "C:/Users/weike/Desktop/0413_data/2_with_timestamp/result" + pic_num + ".png";

                // 求出时间戳并发送物块信息
                for (int j = 0; j < contours.Length; j++)
                {
                    int i = contours.Length - j - 1;
                    if (contours.Length > 50)
                        break;
                    double area = Cv2.ContourArea(contours[i]);
                    if (area == 0 || boundRect[i].Height > row / 3 || boundRect[i].Width < 10 || boundRect[i].Width > num_of_mouth / 2 || boundRect[i].Height < 10)
                        continue;

                    wr2.WriteLine(Convert.ToString(stamp) + ':' + Convert.ToString(stamp.Millisecond) + '\t' + contours.Length + '\t' + boundRect[i].X + '\t' + boundRect[i].Y + '\t' + boundRect[i].Width + '\t' + boundRect[i].Height);
                    //341 347 348 346
                    wr2.Flush();

                    flag = 1;
                    Data_Set data = new Data_Set();                                 // 发送数据包
                    // data.flow_num = Convert.ToString(total_card_num % 1000);        // 设置流水编号
                    data.Init_Dataset(boundRect[i], ximagew);                       // 初始化数据包为可吹气  

                    /*
                    // 设置开始喷吹时间+780-120
                    int k = (int)((boundRect[i].Y * integral_time) - integral_time * 512);
                    data.start_time[0] = (byte)(stamp.Year - 2000);
                    data.start_time[1] = (byte)(stamp.Month);
                    data.start_time[2] = (byte)(stamp.Day);
                    data.start_time[3] = (byte)(stamp.Hour);
                    data.start_time[4] = (byte)(stamp.Minute);
                    if (stamp.Millisecond + k < 0)
                    {
                        data.start_time[5] = (byte)(stamp.Second + k/1000 - 1);
                        data.millionseconds = (byte)(stamp.Millisecond + 1000 + k % 1000);
                    }
                    else
                    {
                        data.start_time[5] = (byte)(stamp.Second);
                        data.millionseconds = (byte)(stamp.Millisecond + k);
                    }
                    if (data.start_time[5] < 0)
                    {
                        data.start_time[5] = (byte)(60 + stamp.Second);
                        data.start_time[4] = (byte)(stamp.Minute - 1);
                    }
                    if (data.start_time[4] < 0)
                    {
                        data.start_time[4] = (byte)(60 + stamp.Minute);
                        data.start_time[3] = (byte)(stamp.Hour - 1);
                    }
                    if (data.start_time[3] < 0)
                    {
                        data.start_time[3] = (byte)(24 + stamp.Hour);
                        data.start_time[2] = (byte)(stamp.Hour - 1);
                    }
                    */

                    // 设置开始喷吹时间(使用格林威治毫秒时间)
                    int k = (int)((boundRect[i].Y * integral_time) - integral_time * 512);  // 找出物块在图像中的实际时间
                    TimeSpan time_stamp = stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0);
                    data.start_time_int = (Int64)time_stamp.TotalMilliseconds;
                    data.start_time_int = data.start_time_int + k;                                      // 计算物块到达X光的时间
                    long start_detect_time = data.start_time_int;


                    // 对物块的种类进行判断
                    // while (opFlag != 0)                      // 循环到没有对线阵进行操作
                    // { }
                    //opFlag = 2;                                 // 将当前队列可读写状态设置为2
                    //Queue<GFinfo> gfinfo = info_queue;
                    int queue_flag = 0;                         // 标志当前队列第一个是否与物块信息符合,默认为最新金属还没轮到当前物块
                    int start_num = boundRect[i].X * 10 / col;
                    int end_num = (boundRect[i].X + Width) * 10 / col;
                    while (info_queue.Count != 0 || first_info.flag != 0)
                    {
                        //gfinfo = info_queue;
                        //GFinfo first_info = gfinfo.Dequeue();
                        if (first_info.flag == 0)
                            first_info = info_queue.Dequeue();
                        Int64 a = data.start_time_int - first_info.time;
                        if (Math.Abs(a) <= 25)
                        {
                            if (first_info.channelindex >= start_num && first_info.channelindex <= end_num)
                            {
                                queue_flag = 1;
                                data.typof_block = (byte)first_info.flag;
                            }
                            else
                                queue_flag = 3;
                        }
                        else if (a > 25)    // 如果当前物块时间已经大于当前队列中第一个物块的时间
                        {
                            queue_flag = 2;
                        }
                        if (queue_flag == 1)        // 如果队列信息已经被读取，需要对队列首个物块进行剔除
                        {
                            lock (locker)
                            {
                                wr.WriteLine("Out Flag=1: queue_count = " + info_queue.Count);
                                wr.Flush();
                                first_info.flag = 0;
                            }
                            break;
                        }
                        else if (queue_flag == 2)
                        {
                            lock (locker)
                            {
                                wr.WriteLine("Out Flag = 2: queue_count = " + info_queue.Count);
                                wr.Flush();
                                first_info.flag = 0;        // 如果队列信息已经过期，剔除队列首个物体，然后继续寻找在队列中对下一个金属信息进行核验
                            }
                            //info_queue.Dequeue();   
                            continue;
                        }
                        else if (queue_flag == 3)       // 如果队列由于通道数信息不符合，可以继续寻找队列内的下一个金属信息
                        {
                            lock (locker)
                            {
                                wr.WriteLine("Out Flag = 3: queue_count = " + info_queue.Count);
                                wr.Flush();
                                first_info.flag = 0;        // 如果队列信息已经过期，剔除队列首个物体，然后继续寻找在队列中对下一个金属信息进行核验
                            }
                            continue;
                        }
                        else
                        {
                            break;                      // 如果队列里时间最早的金属信息时间远大于当前物块时间或者通道数不符合，则跳出循环
                        }

                        //opFlag = 0;                 // 将当前对队列读取状态转为可读取改动状态
                    }


                    // 确定物块最终喷吹的时间
                    data.start_time_int += (int)(2400 / speed) - 10;                                    // 计算出物块到达喷嘴的格林威治毫秒时间


                    //data.start_time = (Int64)stamp.TotalMilliseconds + (Int64)(boundRect[i].Y / line_num_persecond);
                    // 设置持续喷吹的时间
                    data.blow_time = (Int16)(boundRect[i].Height * integral_time + 5);
                    //data.blow_time = (short)100;
                    // 设置开始吹气阀号和停止吹气阀号
                    /*
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
                    */

                    data.start_num = (Int16)((float)boundRect[i].X / col * num_of_mouth);
                    data.start_num = (Int16)(data.start_num - 2);
                    //data.start_num = (short)1;
                    if (data.start_num < 1)
                        data.start_num = (short)1;
                    //data.end_num = (Int16)(data.start_num + (float)boundRect[i].Width / col * length_linearray * (float)SOD / SDD / length_belt * num_of_mouth + 1);
                    data.end_num = (Int16)(data.start_num + (float)boundRect[i].Width / col * num_of_mouth);
                    data.end_num = (Int16)(data.end_num + 2);
                    //data.end_num = (short)50;
                    if (data.end_num > num_of_mouth)
                        data.end_num = (short)num_of_mouth;


                    // 让延迟一段时间发送物块信息 
                    // Thread.Sleep((int)(2400/speed) - );
                    // Thread t = new Thread(new ParameterizedThreadStart(thread_for_sending));
                    // wr.WriteLine(t.ManagedThreadId + " thread start!");
                    // t.Start();

                    // 每个物块休眠2ms时间让PLC来得及处理物块信息
                    if (i != 0)
                        Thread.Sleep(2);
                    int num = 0;
                    if (data.typof_block == 1)
                        num = SendData(data);
                    //if (num == 23)
                    //{
                    total_clock_num++;
                    CardNum1.Text = Convert.ToString(total_clock_num) + " blocks is seccessfully send!";
                    CardNum2.Text = "start:" + Convert.ToString(data.start_num) + "," + "end:" + Convert.ToString(data.end_num);
                    CardNum3.Text = Convert.ToString(data.start_time_int) + "ms " + k;
                    CardNum4.Text = Convert.ToString(DateTime.Now.Millisecond - stamp.Millisecond) + "ms";
                    CardNum5.Text = Convert.ToString(integral_time * 512) + "ms per picture";
                    //}


                    wr.WriteLine(Convert.ToString(frame_count) + '\t' + Convert.ToString(data.start_num) + '\t' + Convert.ToString(data.end_num) + '\t' + start_detect_time + '\t' + data.blow_time + "ms\t" + Convert.ToString((DateTime.Now - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + "\t" + data.typof_block + "\t" + queue_flag);

                }
                wr.Flush();
                // 画出原始图像和处理后图像
                if (flag == 1)
                {
                    //Cv2.ImWrite(init_pic, image);
                    ximagew.Save("C:/Users/weike/Desktop/0413_data/2_with_timestamp/TEST" + pic_num + ".txt");
                    Cv2.ImWrite(result_pic, connImage);
                }


            }

            // ushort[,] line_info = get_timestamp_test(ximagew);
            Console.WriteLine(ximagew.DataOffset);
            image.Dispose();
            connImage.Dispose();

        }
        public void getCounters(IntPtr p, int row, int col, MatType type)
        {
            Mat image = new Mat(row, col, type, p);
            Console.WriteLine(p);
            Console.WriteLine(image.At<int>(0, 0));
            IntPtr data = image.Ptr(0);
            /*
              for (int i = 0; i < 10; i++)
              {
                  int k = (int)data;
                  Console.WriteLine(k);
                  data = data + 1;
              }*/


            //Mat connImage = new Mat(100, 100, MatType.CV_8UC3, new Scalar(0, 0, 0));

            //Console.WriteLine("=====================");
            //Console.WriteLine(p);
            //Console.WriteLine(image);
            //Console.WriteLine(img);
            //Console.WriteLine(connImage);
            //Console.WriteLine("=====================");

            //image.CopyTo(connImage);
            /*
            Cv2.Blur(image, image, new OpenCvSharp.Size(10, 10));
            Cv2.CvtColor(image, image, ColorConversionCodes.BGR2GRAY); // 具体看输入图像为几通道，单通道则注释
            Cv2.Canny(image, image, 1000, 20000, 3, false); // 输入图像虚为单通道8位图像

            OpenCvSharp.Point[][] contours;
            HierarchyIndex[] hierarchy;
            Cv2.FindContours(image, out contours, out hierarchy, RetrievalModes.External, ContourApproximationModes.ApproxSimple);

            Rect[] boundRect = new Rect[contours.Length];
            RotatedRect[] box = new RotatedRect[contours.Length];
            //List<OpenCvSharp.Point[]> Resultcontours = new List<OpenCvSharp.Point[]>();


            Console.WriteLine("=====================");
            Console.WriteLine(contours.Length);
            Console.WriteLine("=====================");

            for (int i = 0; i < contours.Length; i++)
            {
                double area = Cv2.ContourArea(contours[i]);

                if (area == 0) continue;

                Scalar color = new Scalar(0, 0, 255);
                Cv2.DrawContours(image, contours, i, color, 2, LineTypes.Link8, hierarchy);

                boundRect[i] = Cv2.BoundingRect(contours[i]);
                Cv2.Rectangle(image, new OpenCvSharp.Point(boundRect[i].X, boundRect[i].Y),
                    new OpenCvSharp.Point(boundRect[i].X + boundRect[i].Width, boundRect[i].Y + boundRect[i].Height),
                    new Scalar(0, 255, 0), 2, LineTypes.Link8);

            }
            */
            image.Dispose();
            //connImage.Dispose();
        }

        void OnError(int err_id, string msg)
        {
            Error.Text = "Error: " + msg;
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

        private void FindDevice_Click(object sender, EventArgs e)
        {
            xsystem = new XSystemW();
            xsystem.LocalIP = LocalAdapter.Text;
            xsystem.OnXError += new XSystemW.DelOnXError(OnError);

            if (xsystem.Open() > 0)
            {
                int dev_num = xsystem.FindDevice();
                if (dev_num > 0)
                {
                    xdevice = xsystem.GetDevice(0);
                    GCUIP.Text = xdevice.IP;
                    CmdPort.Text = xdevice.CmdPort.ToString();
                    ImgPort.Text = xdevice.ImgPort.ToString();

                }
            }
            Power_Amplifier_Load();
            locker = new object();
            open_recv();

        }

        private void Reset_Click(object sender, EventArgs e)
        {
            xdevice.IP = GCUIP.Text;
            xdevice.CmdPort = Convert.ToUInt16(CmdPort.Text);
            xdevice.ImgPort = Convert.ToUInt16(ImgPort.Text);
            xsystem.ConfigureDevice(xdevice);

            //Get updated parameters
            int dev_num = xsystem.FindDevice();
            if (dev_num > 0)
            {
                xdevice = xsystem.GetDevice(0);
                GCUIP.Text = xdevice.IP;
                CmdPort.Text = xdevice.CmdPort.ToString();
                ImgPort.Text = xdevice.ImgPort.ToString();

            }

        }

        private void Recover_Click(object sender, EventArgs e)
        {
            //Restore the default network setting
            xsystem.RecoverDevice();
        }

        void SetIntegralTime()
        {
            ulong pixel_size;
            unsafe
            {
                ulong data;
                xcommand.GetPara(36, &data, 0);
                pixel_size = data;
            }
            float integral = 0;
            integral = ((float)pixel_size / 10) / (speed * SDD / SOD);
            //MessageBox.Show("The pixel size is:" + pixel_size + "/10mm;The integral time is:" + integral + "ms");
            ulong integral_times = (ulong)(integral * 1000);
            xcommand.SetPara(3, integral_times, 0);
            integral_time = integral;
        }

        private void Open_Click(object sender, EventArgs e)
        {
            xdevice = xsystem.GetDevice(0);
            xfactory = new XGigFactoryW();
            xcommand = new XCommandW();
            xcommand.Factory = xfactory;
            xcommand.OnXError += new XCommandW.DelOnXError(OnError);
            xcommand.OnXEvent += new XCommandW.DelOnXEvent(OnEvent2);

            if (xcommand.Open(xdevice) > 0)
            {
                xtransfer = new XFrameTransferW();
                xtransfer.LineNum = Convert.ToUInt32(LineNumber.Text);
                xtransfer.OnXError += new XFrameTransferW.DelOnXError(OnError);
                xtransfer.OnXEvent += new XFrameTransferW.DelOnXEvent(OnEvent1);
                xtransfer.OnXFrameReady += new XFrameTransferW.DelOnFrameReady(OnFrameReady);

                xacquisition = new XAcquisitionW();
                xacquisition.Factory = xfactory;
                xacquisition.Transfer = xtransfer;
                xacquisition.OnXError += new XAcquisitionW.DelOnXError(OnError);
                xacquisition.OnXEvent += new XAcquisitionW.DelOnXEvent(OnEvent1);
                xacquisition.EnableLineInfo = 1;
                //MessageBox.Show("Finished Start the Line Info!");

                if (xacquisition.Open(xdevice, xcommand) > 0)
                {
                    xdisplay = new XDisplayW();
                    xdisplay.Open(xdevice, xtransfer.LineNum, DisWin.Handle, Convert.ToUInt32(PsColor.SelectedIndex));
                    xcorrect = new XOffCorrectW();
                    xcorrect.Open(xdevice);
                }
                //Get serial number of X-GCU
                string sn = xcommand.GetPara(51, 0);
                SN.Text = "SN: " + sn;

                UInt64 para_data;
                unsafe
                {
                    xcommand.GetPara(61, &para_data, 0);
                    GCUType.SelectedIndex = Convert.ToInt32(para_data);
                    xcommand.GetPara(45, &para_data, 0);
                    CardType.SelectedIndex = Convert.ToInt32(para_data);
                    xcommand.GetPara(5, &para_data, 0);
                    OPMode.SelectedIndex = Convert.ToInt32(para_data);
                    xcommand.GetPara(37, &para_data, 0);
                    switch (para_data)
                    {
                        case 16:
                            PixelDepth.SelectedIndex = 0;
                            break;
                        case 18:
                            PixelDepth.SelectedIndex = 1;
                            break;
                    }
                    xcommand.GetPara(43, &para_data, 0);
                    DMPixNum.SelectedIndex = Convert.ToInt32(para_data) - 5;
                    xcommand.GetPara(20, &para_data, 0);
                    BinMode.SelectedIndex = Convert.ToInt32(para_data);

                    xcommand.GetPara(8, &para_data, 0);
                    CardNum1.Text = ((para_data & 0x000000FF00000000) >> 32).ToString();
                    CardNum2.Text = ((para_data & 0x00000000FF000000) >> 24).ToString();
                    CardNum3.Text = ((para_data & 0x0000000000FF0000) >> 16).ToString();
                    CardNum4.Text = ((para_data & 0x000000000000FF00) >> 8).ToString();
                    CardNum5.Text = (para_data & 0x00000000000000FF).ToString();

                    total_card_num = Convert.ToInt32(CardNum1.Text) + Convert.ToInt32(CardNum2.Text) + Convert.ToInt32(CardNum3.Text) + Convert.ToInt32(CardNum4.Text) + Convert.ToInt32(CardNum5.Text);
                    for (int i = 0; i < total_card_num; i++)
                    {
                        string item = "DM" + Convert.ToString(i + 1);
                        DMIndex.Items.Add(item);
                    }
                }


                // 设置初始化参数
                // xcommand.ExecutePara(56, 0);     // 恢复设备的初始化参数
                // MessageBox.Show("Finished Init the X-GCU");
                // xcommand.SetPara(25, 3, 0);     // External line trigger mode value : Async trigger stamp mode
                // xcommand.SetPara(26, 1, 0);     // Enables external line trigger function
                // xcommand.SetPara(30, 512, 0);   // External frame trigger state value
                // xcommand.SetPara(66, 1, 0);     // Trigger stamp parity mode : Use “odd” parity check;
                // xcommand.SendCommand(0x92, 0x01, 0x00, 0x01, "1");     // 设置波特率为9600   

                // 连接到PLC
                Connect_to_PLC();
                //if (client.Connected)
                    //MessageBox.Show("Successfully Connect to PLC!");
                SetIntegralTime();
                fs = new FileStream(result_data, FileMode.Append);
                wr = new StreamWriter(fs);
                fs2 = new FileStream(time_data, FileMode.Append);
                wr2 = new StreamWriter(fs2);

                // 设定定时器

                //AutoCheckTimer.Enabled = true;
                //AutoCheckTimer2.Enabled = true;
                //AutoCheckTimer2.Interval = AutoCheckTimer.Interval;
                //t.Elapsed += new System.Timers.ElapsedEventHandler(timecheck);
                //t.AutoReset = true;
                //t.Enabled = true;

                timerthread = new Thread(timer);
                timerthread.IsBackground = true;

                timerthread.Start();
            }

        }

        private void Close_Click(object sender, EventArgs e)
        {
            if (xdisplay != null)
                xdisplay.Close();
            if (xacquisition != null)
                xacquisition.Close();
            if (xcommand != null)
                xcommand.Close();
            if (client.Connected)
                client.Close();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (xdisplay != null)
                xdisplay.Close();
            if (xacquisition != null)
                xacquisition.Close();
            if (xcommand != null)
                xcommand.Close();
            if (xsystem != null)
                xsystem.Close();
        }

        private void ResetConf_Click(object sender, EventArgs e)
        {
            UInt64 para_data = 0;
            para_data = Convert.ToUInt64(CardNum1.Text);
            para_data = (para_data << 8) + Convert.ToUInt64(CardNum2.Text);
            para_data = (para_data << 8) + Convert.ToUInt64(CardNum3.Text);
            para_data = (para_data << 8) + Convert.ToUInt64(CardNum4.Text);
            para_data = (para_data << 8) + Convert.ToUInt64(CardNum5.Text);
            xcommand.SetPara(8, para_data, 0);

            xcommand.SetPara(5, Convert.ToUInt64(OPMode.SelectedIndex), 0);
            xcommand.SetPara(20, Convert.ToUInt64(BinMode.SelectedIndex), 0);

            //Save all setting to flash
            xcommand.ExecutePara(2, 0);
            MessageBox.Show("Please click \"Close\" and \"Open\"");
        }

        private void RestoreConf_Click(object sender, EventArgs e)
        {
            //Restor the default settings
            xcommand.ExecutePara(56, 0);
            MessageBox.Show("Please click \"Close\" and \"Open\"");
        }

        private void Grab_Click(object sender, EventArgs e)
        {
            frame_count = 0;
            lost_line = 0;
            wr.WriteLine("\n*************************************************************************");
            DateTime dt = DateTime.Now;
            wr.WriteLine("*******************" + Convert.ToString(dt) + "********************");
            wr2.WriteLine("\n*************************************************************************");
            wr2.WriteLine("*******************" + Convert.ToString(dt) + "********************");
            wr.WriteLine("frame_count" + '\t' + "start_num" + '\t' + "end_num" + '\t' + "start_time" + '\t' + '\t' + "blow_time" + '\t' + '\t' + "send_time\ttypof_block\tqueue_flag");
            wr.Flush();
            wr2.WriteLine("frame_count\tcontour_length\tstart_X\tstart_Y\tWidth\tHeight");
            wr2.Flush();
            LostLine.Text = "Lost Line: " + Convert.ToString(lost_line);
            xacquisition.Grab(0);
            Console.WriteLine("start grab!!!");
            if (check_time_num != 0)
            {
                //AutoCheckTimer.Enabled = true;
                //AutoCheckTimer2.Enabled = true;
                //t.Enabled = true;
            }

            // 启动接收功放数据线程
            recv = true;
            if (recv)
            {
                CardNum1.Text = "Start Receive!";
                recv_thread = new Thread(recv_data);
                recv_thread.IsBackground = true;
                recv_thread.Start();
            }

            first_info.flag = 0;
            first_info.channelindex = 0;
            first_info.time = 0;

        }

        private void Stop_Click(object sender, EventArgs e)
        {
            //wr.Close();
            //AutoCheckTimer.Enabled = false;
            //AutoCheckTimer2.Enabled = false;
            //t.Enabled = false;
            timerthread.Abort();
            recv = false;
            //recv_thread.Abort();
            xacquisition.Stop();
        }

        private void GamaPlus_Click(object sender, EventArgs e)
        {
            xdisplay.Gama += 0.2f;
        }

        private void GamaSub_Click(object sender, EventArgs e)
        {
            xdisplay.Gama -= 0.2f;
        }

        private void SetIntTime_Click(object sender, EventArgs e)
        {
            xcommand.SetPara(3, Convert.ToUInt64(IntTime.Text), 0);
        }

        private void SetNonIntTime_Click(object sender, EventArgs e)
        {
            xcommand.SetPara(4, Convert.ToUInt64(NonIntTime.Text), 0);
        }

        private void GCUTestPattern_SelectedIndexChanged(object sender, EventArgs e)
        {
            xcommand.SetPara(41, Convert.ToUInt64(GCUTestPattern.SelectedIndex), 0);
        }

        private void EnableLED_CheckedChanged(object sender, EventArgs e)
        {
            xcommand.SetPara(57, Convert.ToUInt64(EnableLED.Checked), 0);
        }

        private void SendCmd_Click(object sender, EventArgs e)
        {
            string recv_str = xcommand.SendAscCommand(CommandText.Text.ToString());
            CommandText.Text += recv_str;
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (xcommand != null)
            {
                UInt64 para_data;
                unsafe
                {
                    xcommand.GetPara(3, &para_data, 0);
                    IntTime.Text = Convert.ToString(para_data);
                    xcommand.GetPara(4, &para_data, 0);
                    NonIntTime.Text = Convert.ToString(para_data);
                    xcommand.GetPara(57, &para_data, 0);
                    EnableLED.Checked = Convert.ToBoolean(para_data);
                    xcommand.GetPara(41, &para_data, 0);
                    GCUTestPattern.SelectedIndex = Convert.ToInt32(para_data);

                    xcommand.GetPara(11, &para_data, 0);
                    EnableGO.Checked = Convert.ToBoolean(para_data);
                    xcommand.GetPara(16, &para_data, 0);
                    BaseLine.Text = Convert.ToString(para_data);
                    xcommand.GetPara(13, &para_data, 0);
                    EnableBS.Checked = Convert.ToBoolean(para_data);

                    xcommand.GetPara(26, &para_data, 0);
                    EnableExLine.Checked = Convert.ToBoolean(para_data);
                    xcommand.GetPara(25, &para_data, 0);
                    LineTriggerMode.SelectedIndex = Convert.ToInt32(para_data);
                    xcommand.GetPara(27, &para_data, 0);
                    LineFineD.Text = Convert.ToString(para_data);
                    xcommand.GetPara(28, &para_data, 0);
                    LineRawD.Text = Convert.ToString(para_data);

                    xcommand.GetPara(30, &para_data, 0);
                    if (para_data > 0)
                        EnableExFrame.Checked = Convert.ToBoolean(1);
                    else
                        EnableExFrame.Checked = Convert.ToBoolean(0);
                    xcommand.GetPara(29, &para_data, 0);
                    FrameTriggerMode.SelectedIndex = Convert.ToInt32(para_data);
                    xcommand.GetPara(31, &para_data, 0);
                    FrameD.Text = Convert.ToString(para_data);

                    xcommand.GetPara(21, &para_data, 0);
                    AverageFilter.SelectedIndex = Convert.ToInt32(para_data);
                    xcommand.GetPara(22, &para_data, 0);
                    SumFilter.SelectedIndex = Convert.ToInt32(para_data);
                }
            }
        }

        private void DMIndex_SelectedIndexChanged(object sender, EventArgs e)
        {
            UInt64 para_data;
            cur_dm_index = Convert.ToUInt32(DMIndex.SelectedIndex + 1);
            unsafe
            {
                xcommand.GetPara(6, &para_data, cur_dm_index);
                LowGain.Text = (para_data & 0x00000000000000FF).ToString();
                HighGain.Text = ((para_data & 0x000000000000FF00) >> 8).ToString();

                xcommand.GetPara(42, &para_data, cur_dm_index);
                DMTestPattern.SelectedIndex = Convert.ToInt32(para_data);

                XHealthParaW dm_health_para = xcommand.GetHealth(60, cur_dm_index);
                HealthInfo.Text = "Health Info: \n" + "Temperature: " + Convert.ToString(dm_health_para.temperature) + "\n"
                    + "High Precision Temperature: " + Convert.ToString(dm_health_para.high_temp) + "\n"
                    + "Humidity: " + Convert.ToString(dm_health_para.humidity) + "\n"
                    + "V1: " + Convert.ToString(dm_health_para.v1) + "\n"
                    + "V2: " + Convert.ToString(dm_health_para.v2) + "\n"
                    + "V3: " + Convert.ToString(dm_health_para.v3) + "\n"
                    + "V4: " + Convert.ToString(dm_health_para.v4) + "\n"
                    + "V5: " + Convert.ToString(dm_health_para.v5) + "\n"
                    + "V6: " + Convert.ToString(dm_health_para.v6) + "\n"
                    + "V7: " + Convert.ToString(dm_health_para.v7);

            }
        }

        private void DMTestPattern_SelectedIndexChanged(object sender, EventArgs e)
        {
            xcommand.SetPara(42, Convert.ToUInt64(DMTestPattern.SelectedIndex), cur_dm_index);
        }

        private void ResetADM_Click(object sender, EventArgs e)
        {
            UInt64 para_data = 0;
            para_data = Convert.ToUInt64(HighGain.Text);
            para_data = (para_data << 8) + Convert.ToUInt64(LowGain.Text);
            xcommand.SetPara(6, para_data, cur_dm_index);
        }

        private void ResetAllDMGain_Click(object sender, EventArgs e)
        {
            UInt64 para_data = 0;
            para_data = Convert.ToUInt64(HighGain.Text);
            para_data = (para_data << 8) + Convert.ToUInt64(LowGain.Text);
            xcommand.SetPara(6, para_data, 0xFF);
        }

        private void CalculateOffset_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Please turn off X-Ray");
            xcorrect.Reset();
            xcorrect.CalculatePara(1, xacquisition, xtransfer, 0);
        }

        private void CalculateGain_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Please turn on X-Ray");
            xcorrect.CalculatePara(0, xacquisition, xtransfer, Convert.ToUInt32(CorrectTarget.Text));
        }

        private void SaveGO_Click(object sender, EventArgs e)
        {
            xcommand.StopHeartbeat();
            xcorrect.SaveFlash(xcommand, Convert.ToByte(FlashIndex.Text));
            xcommand.StartHeartbeat();
        }

        private void LoadGO_Click(object sender, EventArgs e)
        {
            xcorrect.LoadFlash(xcommand, Convert.ToByte(FlashIndex.Text));
        }

        private void EnableGO_CheckedChanged(object sender, EventArgs e)
        {
            xcommand.SetPara(11, Convert.ToUInt64(EnableGO.Checked), 0);
            xcommand.SetPara(12, Convert.ToUInt64(EnableGO.Checked), 0);
        }

        private void EnableBS_CheckedChanged(object sender, EventArgs e)
        {
            xcommand.SetPara(16, Convert.ToUInt64(BaseLine.Text), 0);
            xcommand.SetPara(13, Convert.ToUInt64(EnableBS.Checked), 0);
        }

        private void EnableExLine_CheckedChanged(object sender, EventArgs e)
        {
            xcommand.SetPara(26, Convert.ToUInt64(EnableExLine.Checked), 0);
        }

        private void LineTriggerMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            xcommand.SetPara(25, Convert.ToUInt64(LineTriggerMode.SelectedIndex), 0);
        }

        private void LineFineDelay_Click(object sender, EventArgs e)
        {
            xcommand.SetPara(27, Convert.ToUInt64(LineFineD.Text), 0);
        }

        private void LineRawDelay_Click(object sender, EventArgs e)
        {
            xcommand.SetPara(28, Convert.ToUInt64(LineRawD.Text), 0);
        }

        private void EnableExFrame_CheckedChanged(object sender, EventArgs e)
        {
            if (EnableExFrame.Checked)
                xcommand.SetPara(30, Convert.ToUInt64(LineNumber.Text) / 32, 0);
            else
                xcommand.SetPara(30, 0, 0);
        }

        private void FrameTriggerMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            xcommand.SetPara(29, Convert.ToUInt64(FrameTriggerMode.SelectedIndex), 0);
        }

        private void FrameDelay_Click(object sender, EventArgs e)
        {
            xcommand.SetPara(31, Convert.ToUInt64(FrameD.Text), 0);
        }

        private void AverageFilter_SelectedIndexChanged(object sender, EventArgs e)
        {
            xcommand.SetPara(21, Convert.ToUInt64(AverageFilter.SelectedIndex), 0);
        }

        private void SumFilter_SelectedIndexChanged(object sender, EventArgs e)
        {
            xcommand.SetPara(22, Convert.ToUInt64(SumFilter.SelectedIndex), 0);
        }

        private void ImgPort_TextChanged(object sender, EventArgs e)
        {

        }

        private void LocalAdapter_TextChanged(object sender, EventArgs e)
        {

        }

        private void CardNum1_TextChanged(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void Power_Amplifier_Load()
        {
            msg_queue = new Queue<Msg>();
            conditional_variable = new AutoResetEvent(false);
            gflocker = new object();

            udpRecv = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);                     //接收功放设备udp数据报   25ms10个包（10个通道每个通道25ms发一个包）
            endpoint = new IPEndPoint(IPAddress.Parse(PowerIP), 27001);          //172.28.110.50   27001
            udpRecv.Bind(endpoint);

            udpSend = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);                    //给HJ负责设备发送数据报
            sendpoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 1130);
            //udpSend.Bind(sendpoint);
            udpSend.Connect(sendpoint);

           


        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //GetTXT_as_mat(test_txt_filepath);
            //OnFrameReady_testimage();
            //Total_Block_Num.Text = Convert.ToString((byte)(DateTime.Now.Year - 2000));
            timecheck();

        }

        private void TestPLC_Click(object sender, EventArgs e)
        {
            if (client == null)
            {
                client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            }
            if (!client.Connected)
            {
                // OperateResult connect = omronFinisNet.ConnectServer();
                byte[] ntp_testdata = new byte[48];

                IPAddress ip = IPAddress.Parse(ntpServer);
                IPEndPoint remoteep = new IPEndPoint(ip, 6000);
                // AsyncCallback callback = new AsyncCallback(ConnectCallback);
                client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                client.Connect(remoteep);
                if (client.Connected)
                    Total_Block_Num.Text = "Successfully Connect!";
                else
                    Total_Block_Num.Text = "Unsuccessfully Connect!";
                client.ReceiveTimeout = 3000;
                client.Send(ntp_testdata);
                // client.Receive(ntp_testdata);
                client.Close();
                /*if (connect.IsSuccess)
                    MessageBox.Show("连接成功!");
                else
                    MessageBox.Show("连接失败!");*/
            }
        }

        private void AutoCheckTimer_Tick(object sender, EventArgs e)
        {
            //timecheck();
        }

        private void startGF_Click(object sender, EventArgs e)
        {
            listen_thread = new Thread(RecvMessage);
            process_thread1 = new Thread(ProcessMessage);

            listen_thread.Start();
            process_thread1.Start();
        }

        private void stopGF_Click(object sender, EventArgs e)
        {
            quit_flag = true;
            udpRecv.Close();
            listen_thread.Join();
            lock (locker)
            {
                msg_queue.Clear();
                conditional_variable.Set();
                //conditional_variable.Set();
                //conditional_variable.Set();
                //conditional_variable.Set();
            }
            process_thread1.Join();
            //process_thread2.Join();
            //process_thread3.Join();
            //process_thread4.Join();
            //write(SCAData);
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
                conditional_variable.Set();
            }
        }

        private void ProcessMessage()
        {
            while (!quit_flag)
            {
                List<Msg> msgs = new List<Msg>();
                conditional_variable.WaitOne();
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
                msg.processTime = DateTime.Now;
                Interlocked.Increment(ref intocount);
                //label3.Text = intocount.ToString();

                /*
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
                */

                if (dp.SCACount[0] > gap)           //符合条件的数据报，发送给HJ
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

        public void write(int[,,] arr)             //保存txt文本
        {
            string temppath = @"D:\422test\data";


            for (int k = 0; k < CycleCount / 10; k++)
            {
                for (int i = 0; i < 10; ++i)
                {
                    if (arr[k, i, 2] > gap || arr[k, i, 5] > gap || arr[k, i, 8] > gap)
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

        private void AutoCheckTimer2_Tick(object sender, EventArgs e)
        {
            check_time_num2++;
            //if (check_time_num2 % 100 == 0)
            //timeBox1.Text = Convert.ToString(DateTime.Now) + ":" + Convert.ToString(DateTime.Now.Millisecond);
            //timeBox1.Text = "The machine has worked for " + Convert.ToString(check_time_num2) + " ms";
            //if (check_time_num == check_time_num2)
            //timeBox1.Text = "num1: " + Convert.ToString(check_time_num) + "; num2: " + Convert.ToString(check_time_num2);
        }

        // 退出窗口
        private void Form1_FormClosing(object sender, CancelEventArgs e)
        {
            if (MessageBox.Show("是否要关闭", "退出", MessageBoxButtons.OKCancel, MessageBoxIcon.Question) == DialogResult.OK)
                e.Cancel = true;    // 这里表示退出
        }
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
            }


            return Chnidx;

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
                if (i >= 0 && i < 3)             //根据SCA个数要进行修改
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
}
