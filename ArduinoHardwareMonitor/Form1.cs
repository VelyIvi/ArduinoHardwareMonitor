using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using OpenHardwareMonitor.Hardware;

namespace ArduinoHardwareMonitor
{
    public partial class Form1 : Form
    {
        Computer c = new Computer()
        {
            GPUEnabled = true,
            CPUEnabled = true,
            RAMEnabled = true
        };

        float gpuTemp, gpuUtil, cpuTemp, cpuUtil, ramUsed, ramUtil;

        SerialPort port = new SerialPort();

        public Form1()
        {
            InitializeComponent();
            Init();

        }

        private void Init()
        {
            try
            {
                notifyIcon1.Visible = false;
                port.Parity = Parity.None;
                port.StopBits = StopBits.One;
                port.DataBits = 8;
                port.Handshake = Handshake.None;
                port.RtsEnable = true;
                string[] ports = SerialPort.GetPortNames();
                foreach (string port in ports)
                {
                    comboBox1.Items.Add(port);
                }
                port.BaudRate = 9600;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            c.Open();
        }
        private void Status()            
        {

            foreach (var hardware in c.Hardware)
            {
                if (hardware.HardwareType == HardwareType.GpuNvidia)
                {
                    hardware.Update();
                    foreach (var sensor in hardware.Sensors)
                    {
                        if (sensor.SensorType == SensorType.Temperature)
                        {
                            gpuTemp = sensor.Value.GetValueOrDefault();
                        }
                        else if (sensor.SensorType == SensorType.Load && sensor.Name.Contains("GPU Core"))
                        {
                            gpuUtil = sensor.Value.GetValueOrDefault();
                            //if (Math.Round(add) != 0)
                            //{
                            //    gpuUtilTotal += add;
                            //    gpuUtilTotalAmout++;
                            //}
                        }
                    }
                }
                if (hardware.HardwareType == HardwareType.CPU)
                {
                    hardware.Update();
                    foreach (var sensor in hardware.Sensors)
                    {
                        if (sensor.SensorType == SensorType.Temperature && sensor.Name.Contains("CPU Package"))
                        {
                            cpuTemp = sensor.Value.GetValueOrDefault();
                        }
                        else if (sensor.SensorType == SensorType.Load && sensor.Name.Contains("CPU Total"))
                        {
                            cpuUtil = sensor.Value.GetValueOrDefault();
                        }
                    }
                }
                if (hardware.HardwareType == HardwareType.RAM)
                {
                    hardware.Update();
                    foreach(var sensor in hardware.Sensors)
                    {
                        if(sensor.SensorType == SensorType.Data && sensor.Name.Contains("Used Memory"))
                        {
                            ramUsed = sensor.Value.GetValueOrDefault();
                        } else if (sensor.SensorType == SensorType.Load && sensor.Name.Contains("Memory"))
                        {
                            ramUtil = sensor.Value.GetValueOrDefault();
                            ramUtil = Convert.ToInt32(ramUtil);
                        }

                    }
                }
            }
            try
            {
                int retCpuTemp = Math.Min(Convert.ToInt32(cpuTemp), 99);
                int retCpuUtil = Math.Min(Convert.ToInt32(cpuUtil), 99);

                int retGpuTemp = Math.Min(Convert.ToInt32(gpuTemp), 99);
                int retGpuUtil = Math.Min(Convert.ToInt32(gpuUtil), 99);

                
                int retRamUsed = Math.Min(Convert.ToInt32(ramUsed), 99);
                int retRamUtil = Math.Min(Convert.ToInt32(ramUtil), 99);

                System.Diagnostics.Debug.WriteLine(",CT" + retCpuTemp.ToString("D2") + ",CU" + retCpuUtil.ToString("D2") + ",GT" + retGpuTemp.ToString("D2") + ",GU" + retGpuUtil.ToString("D2") + ",MS" + retRamUsed.ToString("D2") + ",MT" + retRamUtil.ToString("D2") + ",");
                port.Write(",CT"+ retCpuTemp.ToString("D2") + ",CU"+ retCpuUtil.ToString("D2") +",GT"+retGpuTemp.ToString("D2") + ",GU"+ retGpuUtil.ToString("D2") +",MS"+ retRamUsed.ToString("D2") + ",MT" + retRamUtil.ToString("D2") + ",");
            }
            catch (Exception ex)
            {
                timer1.Stop();
                MessageBox.Show(ex.Message);
                toolStripStatusLabel1.Text = "Arduino's not responding...";
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Status();
        }

        private void notifyIcon1_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            this.Show();
            this.WindowState = FormWindowState.Normal;
            notifyIcon1.Visible = false;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                port.Write(",DIS,");
                port.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            label2.Text = "Disconnected";
            timer1.Enabled = false;
            toolStripStatusLabel1.Text = "Connect to Arduino...";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                if (!port.IsOpen)
                {
                    port.PortName = comboBox1.Text;
                    port.Open();
                    timer1.Interval = Convert.ToInt32(comboBox2.Text);
                    timer1.Enabled = true;
                    toolStripStatusLabel1.Text = "Sending data...";
                    label3.Text = "Connected";
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
    }
}
