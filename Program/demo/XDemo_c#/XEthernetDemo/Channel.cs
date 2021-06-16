using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace XEthernetDemo
{
    public class Parset
    {
        /// <summary>
        /// 
        /// </summary>
        public int FineGainTrim { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double GapTime { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int PileupInspectionIntervalOffset { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int PeakCaptureMode { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double PeakSamplingTimeOffset { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double FastTriggerPeakingTime { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int FastTriggerGapTime { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double FastTriggerMaxWidth { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int TriggerThreshold { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int BaselineThreshold { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int EnergyThreshold { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int BaselineAverageLength { get; set; }
    }

    public class SingleChannelAnalyzerConfigItem
    {
        /// <summary>
        /// 
        /// </summary>
        public int StartBin { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int Length { get; set; }
    }

    public class ChannelSettings
    {
        /// <summary>
        /// 
        /// </summary>
        public int EnableChannel { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int SendSpectralMetaData { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int SendSpectrum { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int SendDiagnosticData { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int SendSingleChannelAnalyzerData { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int Gain { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int SwitchedGain { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int BytesPerBin { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int NumberOfBins { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int StartBin { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public int DecimationFactor { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public double PeakingTime { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public Parset Parset { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public List<SingleChannelAnalyzerConfigItem> SingleChannelAnalyzerConfig { get; set; }
    }

    public class ChannelInformation
    {
        /// <summary>
        /// 
        /// </summary>
        public List<double> PeakingTimes { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string DppSerialNumber { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public string DppVersion { get; set; }
    }

    public class Channel
    {
        /// <summary>
        /// 
        /// </summary>
        public ChannelSettings Settings { get; set; }
        /// <summary>
        /// 
        /// </summary>
        public ChannelInformation Information { get; set; }
    }
}
