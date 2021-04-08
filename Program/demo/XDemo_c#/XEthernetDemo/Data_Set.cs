using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace XEthernetDemo
{
    class Data_Set
    {
        string syn_code;    // 同步码(1 btye)
        string flow_num;    // 流水编号(4 byte)
        int typof_block;    // 物块类型：0：镁，1：铝，2：重金属(1 byte)
        bool blow;          // 是否吹气(1 byte)
        string start_time;  // 开始吹气时间(8 byte)
        string end_time;    // 吹气持续时间(2 byte)
        int start_num;      // 开始吹气阀号(2 byte)
        int end_num;        // 结束吹气阀号(2 byte)
        string reserve;     // 保留(1 byte)
        int check_num;      // 校验值(1 byte)

        
    }
}
