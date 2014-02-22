word ChangeAddress(byte NewAddr1, byte NewAddr2) {
  Serial.println("> Change address");

  i2c_start_wait(0 + I2C_WRITE);      // Send start condition and write bit.
  i2c_write(0x2E);                    // Send command for device to return 
                                      // address (0x2E).
  i2c_write(0x00);                    // Send low byte zero to erase.
  i2c_write(0x00);                    // Send high byte zero to erase.
  if (i2c_write(0x6F) == 0) {
    i2c_stop();                       // Release bus, end transaction.
    Serial.println("  Data erased."); // Address erase confirmation message.
  }
  else {
    i2c_stop();                       // Release bus, end transaction.
    Serial.println("  Failed to erase data");
    return -1;
  }

  Serial.print("  Writing data: ");
  Serial.print(NewAddr1, HEX);
  Serial.print(", ");
  Serial.println(NewAddr2, HEX);

  for (int a = 0; a != 256; a++) {
    i2c_start_wait(0 + I2C_WRITE);    // Send start condition and write bit.
    i2c_write(0x2E);                  // Send command for device to return 
                                      // address (0x2E).
    i2c_write(NewAddr1);              // Send low byte zero to assign new address.
    i2c_write(NewAddr2);              // Send high byte zero to assign new address.
    if (i2c_write(a) == 0) {
      i2c_stop();                     // Release bus, end transaction.
      delay(100);                     // Wait 10ms.
      Serial.print("Found correct CRC: 0x");
      Serial.println(a, HEX);
      return a;
    }
  }
  i2c_stop();                         // Release bus, end transaction.
  Serial.println("Correct CRC not found");
  return -1;
}

short readTemp(byte dev)
{
  byte buf[2];
  i2c_start_wait(dev+I2C_WRITE);
  i2c_write(0x07);
  
  // read
  i2c_rep_start(dev+I2C_READ);
  buf[0] = i2c_readAck(); //Read 1 byte and then send ack
  buf[1] = i2c_readAck(); //Read 1 byte and then send ack
  i2c_readNak(); // not sure what this is
  i2c_stop();
  short retval = ((buf[0] & 0x007F) << 8) + buf[1];
  
  return retval;
}
  
  
  
