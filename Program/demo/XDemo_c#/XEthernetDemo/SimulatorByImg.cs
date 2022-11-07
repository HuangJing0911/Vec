//#define _TIMER
//#define _MEMORY
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO;
using OpenCvSharp;

namespace XEthernetDemo
{
    public class SimulatorByImg
    {
        string imgListPath;
        int imgCunt = 0;
        System.Timers.Timer recvSimTimer;
        System.Threading.Thread recvThread;
        int itemTotalCunt = 0;
        public int ItemTotalCunt
        {
            get
            {
                return itemTotalCunt;
            }
        }
        int itemChoosedCunt = 0;
        public int ItemChoosedCunt
        {
            get
            {
                return itemChoosedCunt;
            }
        }
        public int MaxHeight
        {
            get
            {
                return 712;
            }
        }
        public int RoiHeight
        {
            get
            {
                return 512;
            }
        }
        public enum SimulatorByImgStatus
        {
            ready,
            running,
            stopped
        }
        SimulatorByImgStatus status = SimulatorByImgStatus.ready;
        public SimulatorByImgStatus Status
        {
            get
            {
                return status;
            }
        }
        public class SimMats
        {
            //#1
            public Mat rawImg;
            public Mat maskImg;
            public OpenCvSharp.Point[][] contours;
            //#2
            public List<Mat> mask = new List<Mat>();
            public List<Mat> tinyItem = new List<Mat>();
            public List<Mat> gloryItem = new List<Mat>();
            int imgIndex = 0;
            public int RoiHeight;
            public SimMats(Mat _rawImg, Mat _maskImg, int _RoiHeight, int minItemArea, int index)
            {
                imgIndex = index;
                RoiHeight = _RoiHeight;
                rawImg = _rawImg.Clone();
                maskImg = _maskImg.Clone();
                //IO
                //ImgMsgRecorder record = new ImgMsgRecorder();
                //record.SetPath(".\\data\\images\\" + "test" + imgIndex + ".txt");

                int row = rawImg.Height;
                int col = rawImg.Width;
                Mat img = maskImg.Clone();
                #region Get Contours
                //img.ConvertTo(img, MatType.CV_32F);
                //Cv2.Normalize(img, img, 1.0, 0, NormTypes.MinMax);
                //img = img * 255;
                //img.ConvertTo(img, MatType.CV_8UC1);
                //Cv2.ImWrite(init_pic, image);

                Mat element = Cv2.GetStructuringElement(MorphShapes.Rect, new OpenCvSharp.Size(3, 3));               //定义核，开运算
                Cv2.MorphologyEx(img, img, MorphTypes.Open, element);                    //进行形态学开运算操作
                element = Cv2.GetStructuringElement(MorphShapes.Rect, new OpenCvSharp.Size(3, 3));               //定义核，开运算
                Cv2.MorphologyEx(img, img, MorphTypes.Close, element);                    //进行形态学闭运算操作
                                                                                          //Cv2.Blur(img, img, new OpenCvSharp.Size(3, 3));
                                                                                          //Cv2.CvtColor(image, image, ColorConversionCodes.BGR2GRAY); // 具体看输入图像为几通道，单通道则注释
                Cv2.Canny(img, img, 10, 80, 3, false); // 输入图像虚为单通道8位图像
                HierarchyIndex[] hierarchy;
                Cv2.FindContours(img, out contours, out hierarchy, RetrievalModes.External, ContourApproximationModes.ApproxSimple);
                #endregion
                #region Get #2

                int i = 0;
                Rect[] boundRect = new Rect[contours.Length];
                int boundNum = contours.Length;
                //Cv2.PutText(imgRgb, String.Format("itemsNum:{0}", boundNum.ToString()),
                //new OpenCvSharp.Point(10, 10),
                //HersheyFonts.HersheySimplex, 0.3, new Scalar(0, 255, 0));
                foreach (OpenCvSharp.Point[] contour in contours)
                {
                    boundRect[i] = Cv2.BoundingRect(contour);                //获取每个contour的框
                                                                             //Console.WriteLine(String.Format(" ButtonY:{0},ImgROI:{1}", boundRect[i].TopLeft.Y, RoiHeight));
                                                                             //if (boundRect[i].TopLeft.Y > RoiHeight)
                                                                             //{
                                                                             //    i++;
                                                                             //    continue;
                                                                             //}
                                                                             //double area = Cv2.ContourArea(contour);
                                                                             //
                                                                             //if (area < minItemArea)
                                                                             //{
                                                                             //
                                                                             //
                                                                             //    i++;
                                                                             //    continue;
                                                                             //}
                                                                             //Cv2.DrawContours(rawImg, contours, i, new Scalar(0, 0, 255), 1);
                                                                             //rawImg.SaveImage(String.Format("img{0}.bmp", imgIndex));
                                                                             //Cv2.DrawContours(imgRgb, contours, i, new Scalar(255, 0, 0), 1);
                    OpenCvSharp.Point point1 = new OpenCvSharp.Point(boundRect[i].BottomRight.X, boundRect[i].BottomRight.Y);
                    OpenCvSharp.Point point2 = new OpenCvSharp.Point(boundRect[i].TopLeft.X, boundRect[i].TopLeft.Y);
                    //Cv2.Rectangle(imgRgb, point2, point1, 100, 1);
                    var m = maskImg[boundRect[i]].Clone();
                    mask.Add(m);
                    var t = rawImg[boundRect[i]].Clone();
                    tinyItem.Add(t);                        //用框裁剪出所有contour的最小mat,生成一个mat[]
                    var gItem = t.Clone();
                    Cv2.BitwiseAnd(t, m, gItem);
                    gloryItem.Add(gItem);
                    i++;
                }

                #endregion
            }


        }

#if _MEMORY
        public delegate void recvDelegate(SimMats img, out int totalItemCunt, out int selectedItemCunt);
#else
        public delegate void recvDelegate(Mat rawImg, Mat maskImg, int RoiHeight, out int totalItemCunt, out int selectedItemCunt);
#endif
        event recvDelegate recvCallback;
        public SimulatorByImg()
        {
            imgListPath = @".\rawImgs杂质2";
            bool s = Directory.Exists(imgListPath);
            var dirs = Directory.GetFiles(imgListPath);
            foreach (var dir in dirs)
            {
                imgCunt++;
            }
            imgCunt /= 2;
            SimInit();
#if _TIMER
            InitTimer(50, SimByImg, out recvSimTimer, false);
#else
            recvThread = new Thread(ThreadingSimByImg);
            recvThread.Start();
#endif
        }
        public SimulatorByImg(string srcPath)
        {
            imgListPath = srcPath + "\\rawImgs";
            bool s = Directory.Exists(imgListPath);
            var dirs = Directory.GetFiles(imgListPath);
            foreach (var dir in dirs)
            {
                imgCunt++;
            }
            imgCunt /= 2;
            SimInit();
            System.Threading.Thread.Sleep(10);
#if _TIMER
            InitTimer(130, SimByImg, out recvSimTimer, false);
#else
            recvThread = new Thread(ThreadingSimByImg);
            recvThread.Start();
#endif
        }
        /*
         
                     string rootPath = ".\\unsorted";
            var dirs = Directory.GetDirectories(rootPath);
         
         
         */


        ~SimulatorByImg()
        {
            ThreadingExitFlag = true;
        }
        delegate void timerUp(object sender, System.Timers.ElapsedEventArgs e);
        void InitTimer(int interval, timerUp eve, out System.Timers.Timer t, bool enable = false)
        {
            t = new System.Timers.Timer(interval);
            t.AutoReset = true;
            t.Enabled = enable;
            t.Elapsed += new System.Timers.ElapsedEventHandler(eve);
        }
        public void Start()
        {


#if _TIMER
            recvSimTimer.Enabled = true;

#else
            ThreadingRunningFlag = true;

#endif
            status = SimulatorByImgStatus.running;
        }
        int exIndex = 0;
        public void Stop()
        {
#if _TIMER
            recvSimTimer.Stop();
#else
            ThreadingRunningFlag = false;
#endif
        }
        public void Reset()
        {
            itemTotalCunt = 0;
            itemChoosedCunt = 0;
            imgIndex = 0;
        }
        public void RegisterImgCallback(recvDelegate e)
        {
            recvCallback += e;
        }
        int imgIndex = 0;
        List<Mat> rawImgs = new List<Mat>();
        List<Mat> maskImgs = new List<Mat>();
        List<SimMats> imgs = new List<SimMats>();
        void SimInit()
        {
            for (int imgIndex = 0; imgIndex < imgCunt; imgIndex++)
            {
                Mat rawImg = Cv2.ImRead(imgListPath + "\\rawImg" + imgIndex.ToString() + ".bmp");
                Cv2.CvtColor(rawImg, rawImg, ColorConversionCodes.RGB2GRAY);
                Mat maskImg = Cv2.ImRead(imgListPath + "\\maskImg" + imgIndex.ToString() + ".bmp");
                Cv2.CvtColor(maskImg, maskImg, ColorConversionCodes.RGB2GRAY);
                SimMats img = new SimMats(rawImg, maskImg, 512, 25, imgs.Count);
#if _MEMORY
                imgs.Add(img);
#else
                rawImgs.Add(rawImg);
                maskImgs.Add(maskImg);
#endif
            }
        }
        void Sim()
        {
            if (imgIndex < imgCunt)
            {
                //Mat rawImg = Cv2.ImRead(imgListPath + "\\rawImg" + imgIndex.ToString() + ".bmp");
                //Cv2.CvtColor(rawImg, rawImg, ColorConversionCodes.RGB2GRAY);
                //Mat maskImg = Cv2.ImRead(imgListPath + "\\maskImg" + imgIndex.ToString() + ".bmp");
                //Cv2.CvtColor(maskImg, maskImg, ColorConversionCodes.RGB2GRAY);
                int a = 0;
                int b = 0;
#if _MEMORY
                recvCallback?.Invoke(imgs[imgIndex], out a, out b);
#else
                recvCallback?.Invoke(rawImgs[imgIndex], maskImgs[imgIndex], 512, out a, out b);
#endif
                itemTotalCunt += a;
                itemChoosedCunt += b;
                imgIndex++;
                //rawImg.Release();
                //maskImg.Release();
                //rawImg.Dispose();
                //maskImg.Dispose();
            }
            else
            {
                status = SimulatorByImgStatus.stopped;
#if _TIMER
                recvSimTimer.Stop();
#else
#endif
            }
        }
        void SimByImg(object sender, System.Timers.ElapsedEventArgs e)
        {
            Sim();
        }
        bool ThreadingRunningFlag = false;
        bool ThreadingExitFlag = false;
        void ThreadingSimByImg()
        {
            while (!ThreadingExitFlag)
            {
                if (ThreadingRunningFlag)
                    Sim();
            }
        }

    }
}
