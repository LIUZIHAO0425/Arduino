using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;


namespace controller_Arduino
{
    public partial class MainWindow : Window
    {
        SerialPort sp = new SerialPort();
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Connect_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                String portName = comportno.Text;
                sp.PortName = portName;
                sp.BaudRate = 115200;
                sp.Open();
                status.Text = "connected";
            }
            catch (Exception)
            {
                MessageBox.Show("please giva a valid port number or check your connection");
            }
        }
        private void Disconnect_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                sp.Close();
                status.Text = "Disconnected";
            }
            catch (Exception)
            {
                MessageBox.Show("First Connect and then disconnect");
            }
        }

        private void straight_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v183c7eff18181818!");
        }
        private void lift_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v2060fefe66260606!");
        }
        private void right_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v04067f7f66646060!");
        }
        private void back_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v1818181818ff7e3c18!");
        }


        private void TEST1_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/vc0c0c00000000000!");
        }
        private void TEST2_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v3030000000000000!");
        }
        private void TEST3_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v0c0c000000000000!");
        }
        private void TEST4_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v0303000000000000!");
        }
        private void TEST5_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v0000c0c000000000!");
        }
        private void TEST6_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v0000303000000000!");
        }
        private void TEST7_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v00000c0c00000000!");
        }
        private void TEST8_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v000030300000000!");
        }
        private void TEST9_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v00000000c0c00000!");
        }

        private void TEST10_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v0000000030300000!");
        }
        private void TEST11_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v000000000c0c0000!");
        }
        private void TEST12_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v0000000003030000!");
        }

        private void TEST13_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v000000000000c0c0!");
        }
        private void TEST14_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v0000000000003030!");
        }
        private void TEST15_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v0000000000000c0c!");
        }
        private void TEST16_Click(object sender, RoutedEventArgs e)
        {
            sp.Write("/v0000000000000303");
        }

    }
}