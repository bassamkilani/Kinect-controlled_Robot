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
using System.Windows.Shapes;
using System.IO.Ports;
using System.Threading;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for serial_port_selection.xaml
    /// </summary>
    public partial class serial_port_selection : Window
    {

        /// <summary>
        /// Creating an object of the serial port class
        /// </summary>
        private SerialPort sp = new SerialPort();
        private MainWindow main_screen = new MainWindow();

        public serial_port_selection()
        {
            InitializeComponent();

            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                ports_combobox.Items.Add(port);
            }
        }

        private void connect_button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                String portName = ports_combobox.SelectedItem.ToString();
                sp.PortName = portName;
                sp.BaudRate = 9600;
                sp.Open();
                status.Content = "Connected Successfully";
                main_screen.sp = this.sp;
                this.Hide();
                Thread.Sleep(200);
                main_screen.Show();
            }
            catch (Exception)
            {

                MessageBox.Show("Please give a valid port number or check your connection");
            }
        }
    }
}
