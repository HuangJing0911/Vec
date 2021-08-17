using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OpenCvSharp;
using XLibWrapper;


namespace XEthernetDemo
{
    public class Data_Set
    {
        public byte syn_code = Convert.ToByte(0xA5);   // 同步码(1 btye)
        public string flow_num = "0000";               // 流水编号(4 byte)
        public byte typof_block = 2;                    // 物块类型：0：铝，1：铜，2：重金属(1 byte)
        public byte blow = 0;                           // 是否吹气(1 byte)
        public byte[] start_time = new byte[6];         // 开始吹气时间(8 byte)
        public Int64 start_time_int = 0;                    // 开始吹气时间
        public Int16 millionseconds = 0;                    // 开始吹气的毫秒数
        public Int16 blow_time = 0;                     // 吹气持续时间(2 byte)
        public Int16 start_num = 0;                    // 开始吹气阀号(2 byte)
        public Int16 end_num = 0;                      // 结束吹气阀号(2 byte)
        public byte reserve = 0;                       // 保留(1 byte)
        public byte check_num = 0;                     // 校验值(1 byte)

        public Data_Set()
        {
            Console.WriteLine("对象已成功创建!");
        }

        public void Init_Dataset(Rect boundRect, XImageW image)
        {
            blow = 1;
        }

        public void Select_type(byte type)
        {
            typof_block = type;
        }

    }
}
