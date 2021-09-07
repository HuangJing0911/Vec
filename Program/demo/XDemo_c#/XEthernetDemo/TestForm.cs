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
using Newtonsoft.Json;

namespace XEthernetDemo
{
    public partial class TestForm : Form
    {
        public TestForm()
        {
            InitializeComponent();
            //PsColor.SelectedIndex = 0;
            AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(CurrentDomain_UnhandleException);
        }

        private void TestForm_Load(object sender, EventArgs e)
        {

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
        int count_thread = 0;
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
        string result_data = "C:/Users/weike/Desktop/0413_data/result-data.txt";
        string time_data = "C:/Users/weike/Desktop/0413_data/frame-time-data.txt";
        FileStream fs;
        StreamWriter wr;
        FileStream fs2;
        StreamWriter wr2;
        public string plc_server;       // PLC IP
        public string ntpServer;      // 线阵IP
        public string ntpServer2 = "127.0.0.1";
        public string PowerIP;        // 功放IP
        Thread timerthread;
        Thread recv_thread;
        static Object locker;
        GFinfo first_info = new GFinfo();

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
                MessageBox.Show("Stop Receive!");
                Thread.CurrentThread.Abort();
            }

        }

        // 三个错误显示程序
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

        void OnFrameReady(XImageW image)
        {
            frame_count++;
            threadCounters(image);
            xdisplay.Display(image);
        }

        void threadCounters(Object obj)
        {
            DateTime stamp = DateTime.Now;
            XImageW image = (XImageW)obj;
            pic_num++;
            string save_file;
            save_file = "C:/Users/weike/Desktop/0413_data/2_with_timestamp/TEST" + pic_num + ".txt";
            int value = Marshal.ReadInt32(image.DataAddr, 0);

            try
            {
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
                };
                time_finish = DateTime.Now.Millisecond;
                Console.WriteLine("==================================");
                Console.WriteLine("read pixel value spend {0} millisecond", time_finish - time_now);
                getCounters_Pixel(image, image_mat, row, col, MatType.CV_16UC1, stamp);
            }
            catch (ThreadAbortException e)
            {
                MessageBox.Show(e.Message);
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

                    // 每个物块休眠2ms时间让PLC来得及处理物块信息
                    if (i != 0)
                        Thread.Sleep(2);
                    int num = 0;
                    if (data.typof_block == 1)
                        num = SendData(data);
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

        private void FindDeviceButton_Click(object sender, EventArgs e)
        {
            xsystem = new XSystemW();
            xsystem.LocalIP = ntpServer;
            xsystem.OnXError += new XSystemW.DelOnXError(OnError);

            if (xsystem.Open() > 0)
            {
                int dev_num = xsystem.FindDevice();
                /*if (dev_num > 0)
                {
                    xdevice = xsystem.GetDevice(0);
                    GCUIP.Text = xdevice.IP;
                    CmdPort.Text = xdevice.CmdPort.ToString();
                    ImgPort.Text = xdevice.ImgPort.ToString();

                }*/
            }

            Power_Amplifier_Load();
            locker = new object();
            open_recv();

            // openclick
            xdevice = xsystem.GetDevice(0);
            xfactory = new XGigFactoryW();
            xcommand = new XCommandW();
            xcommand.Factory = xfactory;
            xcommand.OnXError += new XCommandW.DelOnXError(OnError);
            xcommand.OnXEvent += new XCommandW.DelOnXEvent(OnEvent2);

            if (xcommand.Open(xdevice) > 0)
            {
                xtransfer = new XFrameTransferW();
                xtransfer.LineNum = Convert.ToUInt32("512");
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
                    xdisplay.Open(xdevice, xtransfer.LineNum, DisWin.Handle, Convert.ToUInt32(0));
                    xcorrect = new XOffCorrectW();
                    xcorrect.Open(xdevice);
                }
                //Get serial number of X-GCU
                string sn = xcommand.GetPara(51, 0);
                SN.Text = "SN: " + sn;

                // 参数初始化
                /*UInt64 para_data;
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
                }*/
            }
        }
    }
}
