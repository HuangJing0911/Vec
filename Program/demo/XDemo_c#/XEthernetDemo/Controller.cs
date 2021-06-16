using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XEthernetDemo
{
    public class ConSettings
    {
        /// <summary>
        /// 
        /// </summary>
        public string IpAddress { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string SubnetMask { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string UdpDestinationHost { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int CycleTime { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int LineTestTime { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int MxSystemIndex { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int SyncMode { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int AllowMixedFirmware { get; set; }
    }

    public class ConInformation
    {
        /// <summary>
        /// 
        /// </summary>
        public string MxCpuAppVersion { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxCpuUdpProtocolVersion { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxCpuSerialNumber { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxCpuMacAddress { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxCpuLinuxVersion { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxCpuRuntimeVersion { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxInterfaceEepromState { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxInterfaceHardwareRevision { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxInterfaceItemNumber { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxInterfaceSerialNumber { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxInterfaceManufacturingDate { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxPowerEepromState { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxPowerHardwareRevision { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxPowerItemNumber { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxPowerSerialNumber { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string MxPowerManufacturingDate { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int MxCpuStatusBits { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double MxCpuTemperature { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double MxCpuUpperSurfaceTemperature { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double MxCpuUnderSurfaceTemperature { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double MxCpuLoadCore1 { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double MxCpuLoadCore2 { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double MxPowerTemperature { get; set; }
    }

    public class Controller
    {
        /// <summary>
        /// 
        /// </summary>
        public ConSettings Settings { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public ConInformation Information { get; set; }
    }
}
