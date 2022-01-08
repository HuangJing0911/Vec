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
            Version.Text = version;
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
        int total_clock_num = 0;                // 共检测到物块
        int total_detect_num = 0;               // 共喷吹物块
        Int64 time_now;
        Int64 time_finish;
        int pic_num = 0;
        int check_time_num = 0;                 // 定时器1的计数器
        int check_time_num2 = 0;                // 定时器2的计数器
        float integral_time = 3;                // 默认扫描积分时间3ms
        public int num_of_mouth = 198;          // 喷嘴数量
        public int length_belt = 1016;          // 皮带长度为1000mm
        public int length_linearray = 1120;     // 线阵长度为1200mm
        uint time_interval = 5;                 // 定时器定时时间为5ms
        public float speed = 3.0f;              // 传送带速度
        public float SDD = 914;
        public float SOD = 815;
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
        StreamWriter wr;
        FileStream fs2;
        StreamWriter wr2;
        public string ntpServer = "192.168.250.1";          // PLC IP
        public string arrayServer = "169.254.84.167";       // 线阵IP
        public string powerServer = "172.28.110.50";        // 功放IP
        public string ntpServer2 = "127.0.0.1";
        //OmronFinsNet omronFinisNet = new OmronFinsNet("192.168.250.1", 6001);
        //System.Timers.Timer t = new System.Timers.Timer(5);
        Thread timerthread;
        Thread recv_thread;
        static Object locker;
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
                Error.Text = path + " not exits!";
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
            wr.WriteLine("Update the first time info list! The length of list is: " + (gflist.length) + "(" + gflist.start_channel + "," + gflist.end_channel + ")," + ",queue_count = " + info_queue.Count);
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

        // 定时检测功放当前通道功能是否正常
        public void ChannelStateCheck()
        {
            if (process_thread1.IsAlive)
            {
                for(int i = 0; i < channelStateData.Length; i++)
                {
                    if (channelStateData[i] != 1)
                        OnError(50, "Channel " + i + " error!");
                }
            }
            wr2.WriteLine(DateTime.Now.ToString() + "ChannelStateCheck Finishing!");
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
                        }
                        info_queue = new Queue<GFinfo>(list);
                    }
                    info_queue.Enqueue(info);
                }
                try
                {
                    wr.WriteLine("Receive time: " + info.time.ToString() + ",index: " + info.channelindex.ToString());
                    wr.Flush();
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

            udpRecv = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);                     //接收功放设备udp数据报   25ms10个包（10个通道每个通道25ms发一个包）
            endpoint = new IPEndPoint(IPAddress.Parse(powerServer), 27001);          //172.28.110.50   27001
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

        private void Humidity_Click(object sender, EventArgs e)
        {

        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            DeleteFiles(result_data + "pic/");
            frame_count = 0;
            lost_line = 0;
            total_clock_num = 0;
            total_detect_num = 0;
            DateTime time = DateTime.Now;
            fs = new FileStream(result_data + "result_data " + time.ToString("yyyy-MM-dd") + " " + time.Hour + "-" + time.Minute + "-" + time.Second + ".txt", FileMode.Append);
            wr = new StreamWriter(fs);
            fs2 = new FileStream(time_data + "frame-time-data " + time.ToString("yyyy-MM-dd") + " " + time.Hour + "-" + time.Minute + "-" + time.Second + ".txt", FileMode.Append);
            wr2 = new StreamWriter(fs2);
            wr.WriteLine("\n*************************************************************************");
            DateTime dt = DateTime.Now;
            wr.WriteLine("*******************" + Convert.ToString(dt) + "********************");
            wr2.WriteLine("\n*************************************************************************");
            wr2.WriteLine("*******************" + Convert.ToString(dt) + "********************");
            wr.WriteLine("frame_count" + '\t' + "start_num" + '\t' + "end_num" + '\t' + "start_time" + '\t' + '\t' + "blow_time" + '\t' + '\t' + "send_time\ttime1\ttime2\ttypof_block\tqueue_flag\tarea\tblack_location");
            wr.Flush();
            wr2.WriteLine("frame_count\tcontour_length\tstart_X\tstart_Y\tWidth\tHeight");
            wr2.Flush();
            LostLine.Text = "Lost Line: " + Convert.ToString(lost_line);
            xacquisition.Grab(0);
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
            listen_thread = new Thread(RecvMessage);
            process_thread1 = new Thread(ProcessMessage);
            

            listen_thread.IsBackground = true;
            process_thread1.IsBackground = true;
            listen_thread.Start();
            process_thread1.Start();

            // 功放功能定时检测
            //ChannelChecktimer.Enabled = true;

            MessageBox.Show("成功启动线阵与功放！");
            StartButton.Enabled = false;
            StopButton.Enabled = true;
        }

        void threadCounters(Object obj)
        {
            DateTime stamp = DateTime.Now;
            XImageW image = (XImageW)obj;
            pic_num++;
            string save_file;
            save_file = System.Windows.Forms.Application.StartupPath + "/result/TEST" + pic_num + ".txt";
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

        // 物块是否是铜的判断函数(五个点任意一点满足就可以)
        private int Is_Material(XImageW ximagew, int X, int Y, int Height, int Width, int value)
        {
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
            init_pic = System.Windows.Forms.Application.StartupPath + "/result/pic/init" + pic_num + ".png";
            //Cv2.ImWrite(init_pic, image);
            Mat connImage = new Mat(100, 100, MatType.CV_8UC3, new Scalar(0, 0, 0));
            image.CopyTo(connImage);
            Cv2.Blur(image, image, new OpenCvSharp.Size(3, 3));
            //Cv2.CvtColor(image, image, ColorConversionCodes.BGR2GRAY); // 具体看输入图像为几通道，单通道则注释
            Cv2.Canny(image, image, 10, 80, 3, false); // 输入图像虚为单通道8位图像


            OpenCvSharp.Point[][] contours;
            HierarchyIndex[] hierarchy;
            Cv2.FindContours(image, out contours, out hierarchy, RetrievalModes.External, ContourApproximationModes.ApproxSimple);
            connImage.CopyTo(image);

            Rect[] boundRect = new Rect[contours.Length];
            RotatedRect[] box = new RotatedRect[contours.Length];
            //List<OpenCvSharp.Point[]> Resultcontours = new List<OpenCvSharp.Point[]>();


            //Console.WriteLine("=====================");
            //Console.WriteLine(contours.Length);
            //Console.WriteLine("=====================");
            time_finish = DateTime.Now.Millisecond;
            Console.WriteLine("process picture spend {0} millisecond", time_finish - time_now);

            wr2.WriteLine("frame count: " + frame_count.ToString() + ";time: " + Convert.ToString((stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + ";length: " + contours.Length);
            wr2.Flush();
            //ximagew.Save("C:/Users/weike/Desktop/0413_data/2_with_timestamp/TEST" + pic_num + ".txt");

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
                    /*
                    if (contours.Length > 50)
                    {
                        break;
                    }
                    */

                    double area = Cv2.ContourArea(contours[i]);
                    boundRect[i] = Cv2.BoundingRect(contours[i]);
                    if (area == 0 || boundRect[i].Height > row / 3)
                        continue;
                    Scalar color = new Scalar(0, 0, 255);
                    //Cv2.DrawContours(connImage, contours, i, color, 1, LineTypes.Link8, hierarchy);
                    boundRect[i] = Cv2.BoundingRect(contours[i]);
                    Cv2.Rectangle(image, new OpenCvSharp.Point(boundRect[i].X, boundRect[i].Y),
                        new OpenCvSharp.Point(boundRect[i].X + boundRect[i].Width, boundRect[i].Y + boundRect[i].Height),
                        new Scalar(0, 255, 0), 1, LineTypes.Link8);
                    //Cv2.PutText(image, i.ToString(), new OpenCvSharp.Point(boundRect[i].X, boundRect[i].Y),
                    //HersheyFonts.HersheySimplex, 0.5, new Scalar(0, 255, 0));
                    Cv2.PutText(image, i.ToString(), new OpenCvSharp.Point(boundRect[i].X, boundRect[i].Y),
                    HersheyFonts.HersheySimplex, 0.3, new Scalar(0, 255, 0));
                }
                result_pic = System.Windows.Forms.Application.StartupPath + "/result/pic/result" + pic_num + ".png";
                //Cv2.ImWrite(result_pic, connImage);

                // 求出时间戳并发送物块信息
                for (int j = 0; j < contours.Length; j++)
                {
                    int i = contours.Length - j - 1;
                    //if (contours.Length > 50)
                    //break;
                    double area = Cv2.ContourArea(contours[i]);
                    if (area == 0 || boundRect[i].Height > row / 3 || boundRect[i].Width < 5 || boundRect[i].Width > num_of_mouth / 2 || boundRect[i].Height < 4)
                        continue;

                    wr2.WriteLine(Convert.ToString((stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + '\t' + contours.Length + '\t' + boundRect[i].X + '\t' + boundRect[i].Y + '\t' + boundRect[i].Width + '\t' + boundRect[i].Height);
                    //341 347 348 346


                    flag = 1;
                    Data_Set data = new Data_Set();                                 // 发送数据包
                    // data.flow_num = Convert.ToString(total_card_num % 1000);        // 设置流水编号
                    data.Init_Dataset(boundRect[i], ximagew);                       // 初始化数据包为可吹气  

                    bool is_small = false;
                    /*
                    if (area < 200)
                    {
                        is_small = true;
                        data.typof_block = 10;
                    }
                    */


                    // 设置开始喷吹时间(使用格林威治毫秒时间)
                    int k = (int)((boundRect[i].Y * integral_time) - integral_time * 512);  // 找出物块在图像中的实际时间
                    TimeSpan time_stamp = stamp - new DateTime(1970, 1, 1, 0, 0, 0, 0);
                    data.start_time_int = (Int64)time_stamp.TotalMilliseconds;
                    data.start_time_int = data.start_time_int + k;                                      // 计算物块到达X光的时间
                    long start_detect_time = data.start_time_int;


                    DateTime time1 = DateTime.Now;


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
                    //data.start_num = (Int16)((float)boundRect[i].X / col * num_of_mouth);
                    //data.start_num = (Int16)(data.start_num);
                    //data.start_num = (short)1;
                    if (data.start_num < 1)
                        data.start_num = (short)1;
                    data.end_num = (Int16)(data.start_num + (float)boundRect[i].Width / col * length_linearray * (float)SOD / SDD / length_belt * num_of_mouth + 1);
                    //data.end_num = (Int16)(data.start_num + (float)boundRect[i].Width / col * num_of_mouth);
                    //data.end_num = (Int16)(data.end_num);
                    //data.end_num = (short)50;
                    if (data.end_num > num_of_mouth)
                        data.end_num = (short)num_of_mouth;

                    DateTime time2 = DateTime.Now;


                    // 对物块的种类进行判断
                    // while (opFlag != 0)                      // 循环到没有对线阵进行操作
                    // { }
                    //opFlag = 2;                                 // 将当前队列可读写状态设置为2
                    //Queue<GFinfo> gfinfo = info_queue;
                    int queue_flag = 0;                         // 标志当前队列第一个是否与物块信息符合,默认为最新金属还没轮到当前物块
                    int start_num = (int)Math.Ceiling((float)data.start_num * 10 / num_of_mouth);
                    int end_num = (int)Math.Ceiling((float)data.end_num * 10 / num_of_mouth);
                    //while (info_queue.Count != 0 || first_info.flag != 0)
                    //while (!is_small)
                    while (true)
                    {
                        //gfinfo = info_queue;
                        //GFinfo first_info = gfinfo.Dequeue();
                        //if (first_info.flag == 0)
                        //first_info = info_queue.Dequeue();
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
                            if (Math.Abs(a) <= 20)
                            {
                                //if ((first_info.channelindex <= start_num && first_info.channelindex >= end_num) || (gflist.start_channel <= start_num && gflist.end_channel >= end_num))
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
                            /*
                            else if (Math.Abs(a) <= 25 && gflist.start_channel <= end_num && gflist.end_channel >= start_num && Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Height, boundRect[i].Width, 7000) > 0)
                            {
                                queue_flag = 4;
                                data.typof_block = (byte)first_info.flag;
                                break;
                            }
                            */
                            else if (a > 20)    // 如果当前物块时间已经大于当前队列中第一个物块的时间
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
                            wr.WriteLine("Successfully Find: gflist.length = " + gflist.length + ",queue_count = " + info_queue.Count);
                            break;
                        }
                        else if (queue_flag == 2)                   // 如果列表中金属信息时间远小于当前物块时间，需要更新列表信息
                        {
                            lock (locker)
                            {
                                wr.WriteLine("Message Exceed Time Limit: gflist.length = " + gflist.length + ",queue_count = " + info_queue.Count);
                                gflist.is_active = false;        // 如果队列信息已经过期，将当前列表设为未激活状态，下一个循环会当前列表信息更新
                            }
                            //info_queue.Dequeue();   
                        }
                        else if (queue_flag == 3)               // 如果当前列表时间符合但通道数不符合，则说明当前物块不是需要喷吹的目标金属，跳出循环
                        {
                            wr.WriteLine("Channel not meet: gflist.length = " + gflist.length + ",queue_count = " + info_queue.Count);
                            break;
                        }
                        else                                    // 当前物块时间远小于列表时间，则说明当前物块不是需要喷吹的目标金属，跳出循环
                        {
                            //wr.WriteLine("Queue identify error:queue_count = " + info_queue.Count);
                            break;
                        }
                        /*
                        if (Math.Abs(a) <= 18)
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
                                wr.WriteLine("Successfully Find: queue_count = " + info_queue.Count);
                                //wr.Flush();
                                first_info.flag = 0;
                            }
                            break;
                        }
                        else if (queue_flag == 2)
                        {
                            lock (locker)
                            {
                                wr.WriteLine("Message Exceed Time Limit: queue_count = " + info_queue.Count);
                                //wr.Flush();
                                first_info.flag = 0;        // 如果队列信息已经过期，剔除队列首个物体，然后继续寻找在队列中对下一个金属信息进行核验
                            }
                            //info_queue.Dequeue();   
                            continue;
                        }
                        else if (queue_flag == 3)       // 如果队列由于通道数信息不符合，可以继续寻找队列内的下一个金属信息
                        {
                            lock (locker)
                            {
                                wr.WriteLine("Channel not meet: queue_count = " + info_queue.Count);
                                //wr.Flush();
                                //first_info.flag = 100;        // 如果通道数不符合，继续寻找在队列中对下一个金属信息进行核验
                            }
                            break;
                        }
                        else
                        {
                            break;                      // 如果队列里时间最早的金属信息时间远大于当前物块时间或者通道数不符合，则跳出循环
                        }
                        */

                        //opFlag = 0;                 // 将当前对队列读取状态转为可读取改动状态
                    }


                    // 确定物块最终喷吹的时间
                    data.start_time_int += (int)(2400 / speed) - 13;                                    // 计算出物块到达喷嘴的格林威治毫秒时间


                    //data.start_time = (Int64)stamp.TotalMilliseconds + (Int64)(boundRect[i].Y / line_num_persecond);
                    // 设置持续喷吹的时间
                    data.blow_time = (Int16)(boundRect[i].Height * integral_time + 7);
                    //data.blow_time = (short)100;


                    // 让延迟一段时间发送物块信息 
                    // Thread.Sleep((int)(2400/speed) - );
                    // Thread t = new Thread(new ParameterizedThreadStart(thread_for_sending));
                    // wr.WriteLine(t.ManagedThreadId + " thread start!");
                    // t.Start();

                    // 每个物块休眠2ms时间让PLC来得及处理物块信息
                    //if (i != 0)
                    //Thread.Sleep(2);
                    int num = 0;
                    //if ((data.typof_block == 1 && Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Height, boundRect[i].Width, 8000) > 0) || (is_small && Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Height, boundRect[i].Width, 8000) > 0))
                    if (FunctionSelect_NoSelect.Checked || (data.typof_block == 1 && Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Height, boundRect[i].Width, 6500) > 0) || (boundRect[i].Y >= row * 0.9 && Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Height, boundRect[i].Width, 5000) > 0))
                    {
                        num = SendData(data);
                        data.typof_block = 1;
                        Cv2.PutText(image, "blow", new OpenCvSharp.Point(boundRect[i].X+10, boundRect[i].Y),
                        HersheyFonts.HersheySimplex, 0.3, new Scalar(0, 255, 0));
                    }
                        
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
                    wr.WriteLine(Convert.ToString(frame_count) + " " + i.ToString() + '\t' + Convert.ToString(data.start_num) + " " + start_num + '\t' + Convert.ToString(data.end_num) + " " + end_num + '\t' + start_detect_time + '\t' + data.blow_time + "ms\t" + Convert.ToString((DateTime.Now - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + "\t" + Convert.ToString((time1 - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + '\t' + Convert.ToString((time2 - new DateTime(1970, 1, 1, 0, 0, 0, 0)).TotalMilliseconds) + '\t' + data.typof_block + "\t" + queue_flag + "\t" + area + "\t" + Is_Material(ximagew, boundRect[i].X, boundRect[i].Y, boundRect[i].Height, boundRect[i].Width, 8000));

                }
                wr.Flush();
                wr2.Flush();
                // 画出原始图像和处理后图像
                if (flag == 1)
                {
                    Cv2.ImWrite(result_pic, image);
                    ximagew.Save(System.Windows.Forms.Application.StartupPath + "/result/pic/TEST" + pic_num + ".txt");
                    Cv2.ImWrite(init_pic, connImage);
                }


            }

            // ushort[,] line_info = get_timestamp_test(ximagew);
            //Console.WriteLine(ximagew.DataOffset);
            image.Dispose();
            connImage.Dispose();

        }

        private void FindDeviceButton_Click(object sender, EventArgs e)
        {
            xsystem = new XSystemW();
            xsystem.LocalIP = arrayServer;
            xsystem.OnXError += new XSystemW.DelOnXError(OnError);

            if (xsystem.Open() > 0)
            {
                int dev_num = xsystem.FindDevice();
                if (dev_num > 0)
                {
                    xdevice = xsystem.GetDevice(0);
                    string s = xdevice.IP;
                    s = xdevice.CmdPort.ToString();
                    s = xdevice.ImgPort.ToString();

                }
            }

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

                // 连接到PLC
                Connect_to_PLC();
                SetIntegralTime();

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
                conditional_variable.Set();
            }
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            wr.WriteLine("Total_num = " + total_clock_num.ToString() + ", Total_detect: " + total_detect_num.ToString());
            wr.Flush();
            // 暂停线阵
            wr.Close();
            fs.Close();
            wr2.Close();
            fs2.Close();
            //AutoCheckTimer.Enabled = false;
            //AutoCheckTimer2.Enabled = false;
            //t.Enabled = false;
            timerthread.Abort();
            recv = false;
            //recv_thread.Abort();
            xacquisition.Stop();

            // 暂停功放
            quit_flag = true;
            //udpRecv.Close();
            listen_thread.Abort();
            lock (locker)
            {
                msg_queue.Clear();
                conditional_variable.Set();
                //conditional_variable.Set();
                //conditional_variable.Set();
                //conditional_variable.Set();
            }
            process_thread1.Abort();
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
        }

        private void ChannelChecktimer_Tick(object sender, EventArgs e)
        {
            ChannelStateCheck();
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
                if ((FunctionSelect_Cu.Checked && dp.SCACount[1] > 8) || (FunctionSelect_Zn.Checked && dp.SCACount[0] > 12) || (FunctionSelect_Pb.Checked && (dp.SCACount[3] > gap_pb || dp.SCACount[2] > gap_pb)) || (FunctionSelect_Fe.Checked && dp.SCACount[4] > 8))           //符合条件的数据报，发送给HJ
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
    }


}


