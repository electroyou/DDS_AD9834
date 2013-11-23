// Author: Leonardo
using System;
using System.Windows;
using System.ComponentModel;
using System.Globalization;

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
        CultureInfo enUS = new CultureInfo("en-US");
        dds.Connect();
        dds.SetFrequency(float.Parse(txtFreq.Text, enUS));        
        dds.SetAmplitude(float.Parse(txtVout.Text, enUS));
        // Not yet implemented on MCU
        //dds.SetPhase(UInt16.Parse(txtPhase.Text, enUS));
        //dds.SetWaveform((AD9834.WaveForm)cbWaveform.SelectedIndex);
        dds.Disconnect();
      }
      catch (Exception ex)
      {
        MessageBox.Show(ex.ToString());        
      }      
    }
  }
}
