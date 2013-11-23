// Author: Leonardo
using System;
using System.Windows;
using System.ComponentModel;

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
        dds.Connect();
        dds.SetFrequency(UInt16.Parse(txtFreq.Text));
        dds.SetAmplitude(float.Parse(txtVout.Text));
        dds.SetPhase(UInt16.Parse(txtPhase.Text));
        dds.SetWaveform((AD9834.WaveForm)cbWaveform.SelectedIndex);
        dds.Disconnect();
      }
      catch (Exception ex)
      {
        MessageBox.Show(ex.ToString());        
      }      
    }
  }
}
