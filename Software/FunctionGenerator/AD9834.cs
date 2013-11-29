// Author: Leonardo
using System;
using System.Linq;
using System.Threading;
using System.IO.Ports;
using System.Windows;
using System.Diagnostics.Contracts;

namespace FunctionGenerator
{
  public class AD9834
  {
    public enum WaveForm
    {
      SINE = 0,
      TRIANGLE = 1
    };

    static volatile SerialPort _serial;
    volatile bool _isExecutingCmd = false;

    // MCU Commands
    const byte CMD_FREQ = 9;
    const byte CMD_FREQ_REG = 1;
    const byte CMD_PHASE = 2;
    const byte CMD_PHASE_REG = 3;
    const byte CMD_VOUT = 4;
    const byte CMD_WAVEFORM = 5;
    const byte CMD_SWEEP = 6;
    const byte CMD_RESET = 255;

    // MCU Messages
    const byte MSG_DONE = 100;    

    public void Connect()
    {
      //Contract.Assume(!_serial.IsOpen, "Connection is already opened");
      string serialPortname = SerialPort.GetPortNames().First(); // "COM16"
      _serial = new SerialPort("COM17", 57600, Parity.None, 8, StopBits.One);
      _serial.Open();      
    }
    public void Disconnect()
    {      
      WaitExecutingCmd();
      if (_serial.IsOpen)
      {
        _serial.Close();
      }
    }
    void WaitExecutingCmd()
    {
      while (_isExecutingCmd) { Thread.Sleep(1); }
    }
    public void SetFrequency(float frequency)
    {
      Contract.Requires<ArgumentOutOfRangeException>(frequency > 0, "Frequency should be positive");

      _isExecutingCmd = true;
      _serial.WriteByte(CMD_FREQ);
      _serial.WriteFloat(frequency);
      _isExecutingCmd = false;
    }
    public void SetPhase(UInt32 phase)
    {
      _isExecutingCmd = true;
      _serial.WriteByte(CMD_PHASE);
      _serial.WriteUInt32(phase);
      _isExecutingCmd = false;
    }
    public void SetAmplitude(float vout)
    {
      _isExecutingCmd = true;
      _serial.WriteByte(CMD_VOUT);
      _serial.WriteFloat(vout);
      _isExecutingCmd = false;
    }
    public void SetWaveform(WaveForm waveform)
    {
      _isExecutingCmd = true;
      _serial.WriteByte(CMD_WAVEFORM);
      _serial.WriteByte((byte)waveform);      
      _isExecutingCmd = false;
    }
  }
}