﻿// Author: Leonardo
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Diagnostics;
using System.Windows;

namespace FunctionGenerator
{
  public static class SerialExtension
  {
    public static void WriteByte(this SerialPort serial, byte data)
    {            
      serial.Write(new byte[] { data }, 0, 1);                  
    }
    public static void WriteUInt16(this SerialPort serial, UInt16 data)
    {
      serial.WriteBytes(BitConverter.GetBytes(data));
    }
    public static void WriteUInt32(this SerialPort serial, UInt32 data)
    {
      serial.WriteBytes(BitConverter.GetBytes(data));
    }
    public static void WriteFloat(this SerialPort serial, float data)
    {      
      // float -> 32 bit signed exp + 32 bit unsigned frac
      serial.WriteBytes(BitConverter.GetBytes(data));
    }
    public static void WriteBytes(this SerialPort serial, byte[] bytes)
    {      
      for (int b = 0; b < bytes.Length; b++)
      {        
        serial.WriteByte(bytes[b]);            
      }
      serial.BaseStream.Flush();      
    }
  }
}
