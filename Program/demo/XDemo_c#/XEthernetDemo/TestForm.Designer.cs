namespace XEthernetDemo
{
    partial class TestForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.FindDeviceButton = new System.Windows.Forms.Button();
            this.StartButton = new System.Windows.Forms.Button();
            this.StopButton = new System.Windows.Forms.Button();
            this.LostLine = new System.Windows.Forms.Label();
            this.Error = new System.Windows.Forms.Label();
            this.Humidity = new System.Windows.Forms.Label();
            this.Temperature = new System.Windows.Forms.Label();
            this.SN = new System.Windows.Forms.Label();
            this.DisWin = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.DisWin)).BeginInit();
            this.SuspendLayout();
            // 
            // FindDeviceButton
            // 
            this.FindDeviceButton.BackColor = System.Drawing.Color.Gainsboro;
            this.FindDeviceButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FindDeviceButton.ForeColor = System.Drawing.Color.MidnightBlue;
            this.FindDeviceButton.Location = new System.Drawing.Point(782, 89);
            this.FindDeviceButton.Name = "FindDeviceButton";
            this.FindDeviceButton.Size = new System.Drawing.Size(188, 61);
            this.FindDeviceButton.TabIndex = 3;
            this.FindDeviceButton.Tag = "0";
            this.FindDeviceButton.Text = "查找设备";
            this.FindDeviceButton.UseVisualStyleBackColor = false;
            this.FindDeviceButton.Click += new System.EventHandler(this.FindDeviceButton_Click);
            // 
            // StartButton
            // 
            this.StartButton.BackColor = System.Drawing.Color.Gainsboro;
            this.StartButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.StartButton.ForeColor = System.Drawing.Color.MidnightBlue;
            this.StartButton.Location = new System.Drawing.Point(782, 241);
            this.StartButton.Name = "StartButton";
            this.StartButton.Size = new System.Drawing.Size(188, 61);
            this.StartButton.TabIndex = 4;
            this.StartButton.Tag = "0";
            this.StartButton.Text = "启动";
            this.StartButton.UseVisualStyleBackColor = false;
            // 
            // StopButton
            // 
            this.StopButton.BackColor = System.Drawing.Color.Gainsboro;
            this.StopButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.StopButton.ForeColor = System.Drawing.Color.MidnightBlue;
            this.StopButton.Location = new System.Drawing.Point(782, 388);
            this.StopButton.Name = "StopButton";
            this.StopButton.Size = new System.Drawing.Size(188, 61);
            this.StopButton.TabIndex = 5;
            this.StopButton.Tag = "0";
            this.StopButton.Text = "停止";
            this.StopButton.UseVisualStyleBackColor = false;
            // 
            // LostLine
            // 
            this.LostLine.AutoSize = true;
            this.LostLine.Location = new System.Drawing.Point(9, 44);
            this.LostLine.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.LostLine.Name = "LostLine";
            this.LostLine.Size = new System.Drawing.Size(95, 15);
            this.LostLine.TabIndex = 12;
            this.LostLine.Text = "Lost Line: ";
            // 
            // Error
            // 
            this.Error.AutoSize = true;
            this.Error.Location = new System.Drawing.Point(760, 9);
            this.Error.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Error.Name = "Error";
            this.Error.Size = new System.Drawing.Size(55, 15);
            this.Error.TabIndex = 11;
            this.Error.Text = "Error:";
            // 
            // Humidity
            // 
            this.Humidity.AutoSize = true;
            this.Humidity.Location = new System.Drawing.Point(514, 9);
            this.Humidity.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Humidity.Name = "Humidity";
            this.Humidity.Size = new System.Drawing.Size(79, 15);
            this.Humidity.TabIndex = 10;
            this.Humidity.Text = "Humidity:";
            // 
            // Temperature
            // 
            this.Temperature.AutoSize = true;
            this.Temperature.Location = new System.Drawing.Point(241, 9);
            this.Temperature.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Temperature.Name = "Temperature";
            this.Temperature.Size = new System.Drawing.Size(103, 15);
            this.Temperature.TabIndex = 9;
            this.Temperature.Text = "Temperature:";
            // 
            // SN
            // 
            this.SN.AutoSize = true;
            this.SN.Location = new System.Drawing.Point(9, 9);
            this.SN.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.SN.Name = "SN";
            this.SN.Size = new System.Drawing.Size(39, 15);
            this.SN.TabIndex = 8;
            this.SN.Text = "SN: ";
            // 
            // DisWin
            // 
            this.DisWin.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.DisWin.Location = new System.Drawing.Point(30, 89);
            this.DisWin.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.DisWin.Name = "DisWin";
            this.DisWin.Size = new System.Drawing.Size(500, 360);
            this.DisWin.TabIndex = 13;
            this.DisWin.TabStop = false;
            // 
            // TestForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1027, 505);
            this.Controls.Add(this.DisWin);
            this.Controls.Add(this.LostLine);
            this.Controls.Add(this.Error);
            this.Controls.Add(this.Humidity);
            this.Controls.Add(this.Temperature);
            this.Controls.Add(this.SN);
            this.Controls.Add(this.StopButton);
            this.Controls.Add(this.StartButton);
            this.Controls.Add(this.FindDeviceButton);
            this.Name = "TestForm";
            this.Text = "测试系统";
            this.Load += new System.EventHandler(this.TestForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.DisWin)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button FindDeviceButton;
        private System.Windows.Forms.Button StartButton;
        private System.Windows.Forms.Button StopButton;
        private System.Windows.Forms.Label LostLine;
        private System.Windows.Forms.Label Error;
        private System.Windows.Forms.Label Humidity;
        private System.Windows.Forms.Label Temperature;
        private System.Windows.Forms.Label SN;
        private System.Windows.Forms.PictureBox DisWin;
    }
}