using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace XEthernetDemo
{
    public class Data_Set
    {
        Byte syn_code = Convert.ToByte("A5");  // 同步码(1 btye)
        string flow_num = "0000";       // 流水编号(4 byte)
        int typof_block = 2;    // 物块类型：0：镁，1：铝，2：重金属(1 byte)
        bool blow = false;          // 是否吹气(1 byte)
        string start_time = DateTime.Now.ToFileTime().ToString();  // 开始吹气时间(8 byte)
        string end_time = "0";    // 吹气持续时间(2 byte)
        UInt16 start_num = 0;      // 开始吹气阀号(2 byte)
        UInt16 end_num = 0;        // 结束吹气阀号(2 byte)
        string reserve = "0";     // 保留(1 byte)
        Byte check_num = 0;      // 校验值(1 byte)

        public Data_Set()
        {
            Console.WriteLine("对象已成功创建!");
        }


    }
}
