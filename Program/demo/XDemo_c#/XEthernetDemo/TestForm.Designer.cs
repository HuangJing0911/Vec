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
            this.FindDeviceButton.Location = new System.Drawing.Point(772, 89);
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
            this.StartButton.Location = new System.Drawing.Point(772, 240);
            this.StartButton.Name = "StartButton";
            this.StartButton.Size = new System.Drawing.Size(188, 61);
            this.StartButton.TabIndex = 4;
            this.StartButton.Tag = "0";
            this.StartButton.Text = "启动";
            this.StartButton.UseVisualStyleBackColor = false;
            this.StartButton.Click += new System.EventHandler(this.StartButton_Click);
            // 
            // StopButton
            // 
            this.StopButton.BackColor = System.Drawing.Color.Gainsboro;
            this.StopButton.Font = new System.Drawing.Font("微软雅黑", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.StopButton.ForeColor = System.Drawing.Color.MidnightBlue;
            this.StopButton.Location = new System.Drawing.Point(772, 386);
            this.StopButton.Name = "StopButton";
            this.StopButton.Size = new System.Drawing.Size(188, 61);
            this.StopButton.TabIndex = 5;
            this.StopButton.Tag = "0";
            this.StopButton.Text = "停止";
            this.StopButton.UseVisualStyleBackColor = false;
            this.StopButton.Click += new System.EventHandler(this.StopButton_Click);
            // 
            // LostLine
            // 
            this.LostLine.AutoSize = true;
            this.LostLine.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.LostLine.ForeColor = System.Drawing.Color.MidnightBlue;
            this.LostLine.Location = new System.Drawing.Point(33, 45);
            this.LostLine.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.LostLine.Name = "LostLine";
            this.LostLine.Size = new System.Drawing.Size(83, 19);
            this.LostLine.TabIndex = 12;
            this.LostLine.Text = "Lost Line: ";
            // 
            // Error
            // 
            this.Error.AutoSize = true;
            this.Error.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Error.ForeColor = System.Drawing.Color.MidnightBlue;
            this.Error.Location = new System.Drawing.Point(629, 9);
            this.Error.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Error.Name = "Error";
            this.Error.Size = new System.Drawing.Size(50, 19);
            this.Error.TabIndex = 11;
            this.Error.Text = "Error:";
            // 
            // Humidity
            // 
            this.Humidity.AutoSize = true;
            this.Humidity.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Humidity.ForeColor = System.Drawing.Color.MidnightBlue;
            this.Humidity.Location = new System.Drawing.Point(425, 9);
            this.Humidity.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Humidity.Name = "Humidity";
            this.Humidity.Size = new System.Drawing.Size(83, 19);
            this.Humidity.TabIndex = 10;
            this.Humidity.Text = "Humidity:";
            this.Humidity.Click += new System.EventHandler(this.Humidity_Click);
            // 
            // Temperature
            // 
            this.Temperature.AutoSize = true;
            this.Temperature.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Temperature.ForeColor = System.Drawing.Color.MidnightBlue;
            this.Temperature.Location = new System.Drawing.Point(187, 9);
            this.Temperature.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.Temperature.Name = "Temperature";
            this.Temperature.Size = new System.Drawing.Size(111, 19);
            this.Temperature.TabIndex = 9;
            this.Temperature.Text = "Temperature:";
            // 
            // SN
            // 
            this.SN.AutoSize = true;
            this.SN.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.SN.ForeColor = System.Drawing.Color.MidnightBlue;
            this.SN.Location = new System.Drawing.Point(33, 9);
            this.SN.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.SN.Name = "SN";
            this.SN.Size = new System.Drawing.Size(39, 19);
            this.SN.TabIndex = 8;
            this.SN.Text = "SN: ";
            // 
            // DisWin
            // 
            this.DisWin.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.DisWin.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.DisWin.Location = new System.Drawing.Point(30, 89);
            this.DisWin.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.DisWin.Name = "DisWin";
            this.DisWin.Size = new System.Drawing.Size(649, 388);
            this.DisWin.TabIndex = 13;
            this.DisWin.TabStop = false;
            // 
            // TestForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1027, 511);
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