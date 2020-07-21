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

namespace ComPort
{
    public partial class Form1 : Form
    {
        string dataOUT;
        string sendWith;
        string dataIN;
        char[] charArr; 
        public Form1()
        {
            InitializeComponent();
        }

 
        private void Form1_Load(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            
            cBoxCOMPORT.Items.AddRange(ports);

            btnOpen.Enabled = true;
            btnClose.Enabled = false;

          
            sendWith = "Write";
            TextBox.CheckForIllegalCrossThreadCalls = false;
            
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.PortName = cBoxCOMPORT.Text;
                serialPort1.BaudRate = Convert.ToInt32(CBoxBaudRate.Text);
                serialPort1.DataBits = Convert.ToInt32(cBoxDataBits.Text);
                serialPort1.StopBits = (StopBits)Enum.Parse(typeof(StopBits), cBoxStopBits.Text);
                serialPort1.Parity = (Parity)Enum.Parse(typeof(Parity), cBoxParityBits.Text);

                serialPort1.Open();
                progressBar1.Value = 100;
                btnOpen.Enabled = false;
                btnClose.Enabled = true;
                lblStatusCom.Text = "ON";
                
            }

            catch (Exception err)
            {
                MessageBox.Show(err.Message,"Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                btnOpen.Enabled = true;
                btnClose.Enabled = false;
                lblStatusCom.Text = "OFF";
            }
        

        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen)
            {
                serialPort1.Close();
                progressBar1.Value = 0;
                btnOpen.Enabled = true;
                btnClose.Enabled = false;
                lblStatusCom.Text = "OFF";
            }
        }

        private void btnSendData_Click(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen)
            {
              
                
            }
        }

        private void chBoxDtrEnable_CheckedChanged(object sender, EventArgs e)
        {
           
        }

        private void chBoxRTSEnable_CheckedChanged(object sender, EventArgs e)
        {
          
        }

        private void btnClearDataOut_Click(object sender, EventArgs e)
        {
           
        }

        private void tBoxDataOut_TextChanged(object sender, EventArgs e)
        {
           
            
        }

        private void chBoxUsingButton_CheckedChanged(object sender, EventArgs e)
        {
           
        }

        private void tBoxDataOut_KeyDown(object sender, KeyEventArgs e)
        {
           
            
        }

        private void chBoxWriteLine_CheckedChanged(object sender, EventArgs e)
        {
           
            
        }

        private void chBoxWrite_CheckedChanged(object sender, EventArgs e)
        {
           
        }
        int cnt = 0;
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            int bytes = serialPort1.BytesToRead;
            byte[] buffer = new byte[bytes];
            serialPort1.Read(buffer, 0, bytes);
            ShowData(buffer);
            
        }

        private void ShowData(byte[] A)
        {

            if (A[0] == 254)
            {
                if (A[1] == 0)
                {
                    LB_ND.Text = "MẤT KẾT NỐI TỚI CẢM BIẾN";
                    LB_CB1.Text = "MẤT KẾT NỐI TỚI CẢM BIẾN";
                    LB_CB2.Text = "MẤT KẾT NỐI TỚI CẢM BIẾN";
                }
                else
                {

                    uint nd = ((uint)  A[2] << 8) | (uint)A[3];
                    uint cb1 = ((uint) A[4] << 8) | (uint)A[5];
                    uint cb2 = ((uint) A[6] << 8) | (uint)A[7];
                    LB_ND.Text = nd.ToString();
                    LB_CB1.Text = cb1.ToString();
                    LB_CB2.Text = cb2.ToString();
                }
            }
        }

        private void chBoxAlwaysUpdate_CheckedChanged(object sender, EventArgs e)
        {
           
        }

        private void chBoxAddToOldData_CheckedChanged(object sender, EventArgs e)
        {
          
        }

        private void btnClearDataIN_Click(object sender, EventArgs e)
        {
           
        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void label10_Click(object sender, EventArgs e)
        {

        }

        private void BNT__TEST_Click(object sender, EventArgs e)
        {
            //serialPort1.Write("21321");
            LB_CB1.Text = "DA";
        }

        private void label12_Click(object sender, EventArgs e)
        {

        }

        private void label15_Click(object sender, EventArgs e)
        {

        }
    }
}
