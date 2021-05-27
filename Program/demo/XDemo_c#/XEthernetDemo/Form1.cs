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


namespace XEthernetDemo
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            PsColor.SelectedIndex = 0;
        }
        
        XSystemW        xsystem;
        XDeviceW        xdevice;
        XGigFactoryW    xfactory;
        XCommandW       xcommand;
        XFrameTransferW xtransfer;
        XAcquisitionW   xacquisition;
        XDisplayW       xdisplay;
        XOffCorrectW    xcorrect;
        XTifFormatW     xtifform;
        int frame_count = 0;
        int lost_line = 0;
        int total_card_num = 0;
        uint cur_dm_index = 0;
        int total_clock_num = 0;
        int time_now;
        int time_finish;
        int pic_num = 0;
        string result_pic;
        string init_pic;
        Socket client = null;
        public byte[] Rcvbuffer;    //接收字节组数
        delegate void AppendDelegate(string str);
        AppendDelegate AppendString;
        string test_txt_filepath = "C:/Users/96342/Desktop/TEST19.txt";
        const string ntpServer = "192.168.250.110";
        OmronFinsNet omronFinisNet = new OmronFinsNet("192.168.250.1", 9600);
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
            col = col / 2;

            int[,] p1 = new int[row, col]; //数组
            int maxp = 0;
            int minp = 65536;

            for (int i = 0; i < row; i++)  //读入数据并赋予数组
            {
                string line = rd.ReadLine();
                string[] data = line.Split('\t');
                for (int j = 0; j < col; j++)
                {
                    p1[i, j] = int.Parse(data[j]);
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
            for(m = 0; m < row; m++)
            {
                for (n = 0; n < col; n++)
                {
                    float k = ((float)(p1[m, n] - minp)) / (maxp - minp);
                    k = k * 255;
                    p1[m, n] = (int)k;
                    image.Set(m, n,p1[m,n]);
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
            Cv2.ImWrite("C:/Users/96342/Desktop/TEST_PRE.png", image);
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
                IPEndPoint remoteep = new IPEndPoint(ip, 9600);
                // AsyncCallback callback = new AsyncCallback(ConnectCallback);
                client = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                client.Connect(remoteep);
                if (client.Connected)
                    Total_Block_Num.Text = "Successfully Connect!";
                else
                    Total_Block_Num.Text = "Unsuccessfully Connect!";
                client.ReceiveTimeout = 3000;
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
            Cv2.ImWrite("C:/Users/96342/Desktop/TEST_result.bmp", connImage);
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
            XImageW image = (XImageW)obj;
            //XTifFormatW image_tif = new XTifFormatW(image,xdevice);
            pic_num++;
            string save_file;
            save_file = "C:/Users/weike/Desktop/0413_data/1_with_timestamp/TEST" + pic_num + ".txt";
            //string save_tif = "C:/Users/77170/Desktop/0316/pic_data/TEST" + pic_num + ".tif";
            image.Save(save_file);
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
                for (uint i = 0; i < (uint)image.Height; i++)
                    for (uint j = 0; j < (uint)image.Width; j++)
                    {
                        pixelval[i, j] = (uint)image.GetPixelVal(i, j);
                        if (pixelval[i, j] > maxp)
                            maxp = pixelval[i, j];
                        else if (pixelval[i, j] < minp)
                            minp = pixelval[i, j];
                    }
                int m, n, row, col;
                row = (int)image.Height;
                col = (int)image.Width;
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
                Console.WriteLine("read pixel value spend {0} millisecond",time_finish-time_now);
                getCounters_Pixel(image, image_mat, (int)image.Height, (int)image.Width, MatType.CV_16UC1);
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
            for(int i = 0; i < 512; i++)
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
                            line_info[hi,wi] = *(pLineAddr + wi);
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
            ushort[,] line_info = new ushort[(int)image.Height, (int)(image.Width + image.DataOffset/2)];
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
                        for (int wi = 0; wi < (image.Width + image.DataOffset/2); wi++)
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

        public void getCounters_Pixel(XImageW ximagew, Mat image, int row, int col, MatType type)
        {

            time_now = DateTime.Now.Millisecond;
            //Mat image = new Mat(row, col, type,p);
            image.ConvertTo(image, MatType.CV_32F);
            Cv2.Normalize(image, image, 1.0, 0, NormTypes.MinMax);
            image = image * 255;
            image.ConvertTo(image, MatType.CV_8UC1);
            init_pic = "C:/Users/weike/Desktop/0413_data/1_with_timestamp/init" + pic_num + ".png";
            Cv2.ImWrite(init_pic, image);
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
                Total_Block_Num.Text = Convert.ToString(total_card_num);
                // 画出检测的轮廓
                for (int i = 0; i < contours.Length; i++)
                {
                    double area = Cv2.ContourArea(contours[i]);
                    if (area == 0) continue;
                    Scalar color = new Scalar(0, 0, 255);
                    Cv2.DrawContours(connImage, contours, i, color, 2, LineTypes.Link8, hierarchy);
                    boundRect[i] = Cv2.BoundingRect(contours[i]);
                    Cv2.Rectangle(connImage, new OpenCvSharp.Point(boundRect[i].X, boundRect[i].Y),
                        new OpenCvSharp.Point(boundRect[i].X + boundRect[i].Width, boundRect[i].Y + boundRect[i].Height),
                        new Scalar(0, 255, 0), 2, LineTypes.Link8);
                }
                result_pic = "C:/Users/weike/Desktop/0413_data/1_with_timestamp/result" + pic_num + ".png";
                Cv2.ImWrite(result_pic, connImage); 
                // 求出时间戳
                 
            }

            ushort[,] line_info = get_timestamp_test(ximagew);
            Console.WriteLine(ximagew.DataOffset);
            image.Dispose();
            connImage.Dispose();
            
        }
        public void getCounters(IntPtr p, int row, int col, MatType type)
        {
            Mat image = new Mat(row, col, type, p);
            Console.WriteLine(p);
            Console.WriteLine(image.At<int>(0,0));
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

            if (xsystem.Open()>0)
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
                    DMPixNum.SelectedIndex = Convert.ToInt32(para_data)-5;
                    xcommand.GetPara(20, &para_data, 0);
                    BinMode.SelectedIndex = Convert.ToInt32(para_data);

                    xcommand.GetPara(8, &para_data, 0);
                    CardNum1.Text = ((para_data & 0x000000FF00000000)>>32).ToString();
                    CardNum2.Text = ((para_data & 0x00000000FF000000)>>24).ToString();
                    CardNum3.Text = ((para_data & 0x0000000000FF0000)>>16).ToString();
                    CardNum4.Text = ((para_data & 0x000000000000FF00)>>8).ToString();
                    CardNum5.Text = (para_data & 0x00000000000000FF).ToString();

                    total_card_num = Convert.ToInt32(CardNum1.Text) + Convert.ToInt32(CardNum2.Text) + Convert.ToInt32(CardNum3.Text) + Convert.ToInt32(CardNum4.Text) + Convert.ToInt32(CardNum5.Text);
                    for (int i = 0; i < total_card_num; i++)
                    {
                        string item = "DM" + Convert.ToString(i + 1);
                        DMIndex.Items.Add(item);
                    }
                }

                // 设置初始化参数
                xcommand.SetPara(25, 2, 0);     // External line trigger mode value : Sync trigger stamp mode
                xcommand.SetPara(30, 512, 0);   // External frame trigger state value
                xcommand.SetPara(66, 1, 0);     // Trigger stamp parity mode : Use “odd” parity check;
                xcommand.SendCommand(0x92, 0x01, 0x00, 0x01, "1");     // 设置波特率为9600
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
            LostLine.Text = "Lost Line: " + Convert.ToString(lost_line);
            xacquisition.Grab(0);
            Console.WriteLine("start grab!!!");
        }

        private void Stop_Click(object sender, EventArgs e)
        {
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
                    xcommand.GetPara(29, &para_data,0);
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
            cur_dm_index = Convert.ToUInt32( DMIndex.SelectedIndex + 1);
            unsafe
            {
                xcommand.GetPara(6, &para_data, cur_dm_index);
                LowGain.Text = (para_data & 0x00000000000000FF).ToString();
                HighGain.Text =((para_data & 0x000000000000FF00)>>8).ToString();

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

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //GetTXT_as_mat(test_txt_filepath);
            OnFrameReady_testimage();
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
                IPEndPoint remoteep = new IPEndPoint(ip, 9600);
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
    }
}
