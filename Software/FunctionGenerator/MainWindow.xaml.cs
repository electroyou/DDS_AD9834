// Author: Leonardo
using System;
using System.Windows;
using System.ComponentModel;
using System.Globalization;
using System.Threading;
using System.IO.Ports;

namespace FunctionGenerator
{  
  public partial class MainWindow : Window
  {    
    AD9834 dds = new AD9834();
    public MainWindow()
    {
      InitializeComponent();
    }    
    private void btnSet_Click(object sender, RoutedEventArgs e)
    {      
      try
      {
        // Use '.' like decimal separator
        CultureInfo enUS = new CultureInfo("en-US");

        float frequency = float.Parse(txtFreq.Text, enUS);
        float vout = float.Parse(txtVout.Text, enUS);
        AD9834.WaveForm waveform = (AD9834.WaveForm)cbWaveform.SelectedIndex;

        dds.Connect();
        dds.SetFrequency(frequency);
        dds.SetAmplitude(vout);
        dds.SetWaveform(waveform);
        // Not yet implemented on MCU
        //dds.SetPhase(UInt16.Parse(txtPhase.Text, enUS));
        dds.Disconnect();
      }
      catch (Exception ex)
      {
        MessageBox.Show(ex.ToString());        
      }      
    }

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
    }
  }
}
