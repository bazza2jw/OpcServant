#ifndef PIGPIOCPP_H
#define PIGPIOCPP_H

#include <MrlLib/mrllib.h>
#include "pigpio/pigpiod_if2.h"
#include "pigpio/command.h"



namespace MRL {

/*!
 * \brief The PiGpio class
 */
class PiGpio
{
    std::string _host;
    int _handle = -1; // connection to pigpiod server
public:
    PiGpio(const std::string &host, int port);
    ~PiGpio();
    /*!
     * \brief host
     * \return
     */
    const std::string &host() { return _host;}
    /*!
     * \brief handle
     * \return
     */
    int handle() const
    {
        return  _handle;
    } // connection to pigpiod server

    int set_mode( unsigned gpio, unsigned mode)
    {
        return ::set_mode(_handle, gpio,  mode);
    }

    int get_mode( unsigned gpio)
    {
        return  ::get_mode(_handle, gpio);
    }

    int set_pull_up_down(unsigned gpio, unsigned pud)
    {
        return ::set_pull_up_down(_handle, gpio, pud);
    }
    int gpio_read(unsigned gpio)
    {
        return ::gpio_read(_handle, gpio);
    }
    int gpio_write(unsigned gpio, unsigned level)
    {
        return ::gpio_write(_handle, gpio, level);
    }
    int set_PWM_dutycycle( unsigned user_gpio, unsigned dutycycle)
    {
        return  ::set_PWM_dutycycle(_handle, user_gpio,  dutycycle);
    }
    int get_PWM_dutycycle( unsigned user_gpio)
    {
        return ::get_PWM_dutycycle(_handle,user_gpio);
    }
    int set_PWM_range(unsigned user_gpio, unsigned range)
    {
        return ::set_PWM_range(_handle,user_gpio,range);
    }
    int get_PWM_range( unsigned user_gpio)
    {
        return ::get_PWM_range(_handle, user_gpio);
    }
    int get_PWM_real_range( unsigned user_gpio)
    {
        return ::get_PWM_real_range(_handle, user_gpio);
    }
    int set_PWM_frequency( unsigned user_gpio, unsigned frequency)
    {
        return ::set_PWM_frequency(_handle, user_gpio, frequency);
    }
    int get_PWM_frequency(unsigned user_gpio)
    {
        return ::get_PWM_frequency(_handle, user_gpio);
    }
    int set_servo_pulsewidth( unsigned user_gpio, unsigned pulsewidth)
    {
        return ::set_servo_pulsewidth(_handle, user_gpio, pulsewidth);
    }
    int get_servo_pulsewidth( unsigned user_gpio)
    {
        return ::get_servo_pulsewidth(_handle, user_gpio);
    }
    int notify_open()
    {
        return  ::notify_open(_handle);
    }
    int notify_begin(unsigned handle, uint32_t bits)
    {
        return ::notify_begin(_handle,  handle, bits);
    }
    int notify_pause(unsigned handle)
    {
        return ::notify_pause(_handle, handle);
    }
    int notify_close(unsigned handle)
    {
        return ::notify_close(_handle, handle);
    }
    int set_watchdog(unsigned user_gpio, unsigned timeout)
    {
        return ::set_watchdog(_handle,  user_gpio,  timeout);
    }
    int set_glitch_filter( unsigned user_gpio, unsigned steady)
    {
        return ::set_glitch_filter(_handle, user_gpio, steady);
    }
    int set_noise_filter(unsigned user_gpio, unsigned steady, unsigned active)
    {
        return ::set_noise_filter(_handle,  user_gpio,  steady,  active);
    }
    uint32_t read_bank_1()
    {
        return ::read_bank_1(_handle);
    }
    uint32_t read_bank_2()
    {
        return  ::read_bank_2(_handle);
    }
    int clear_bank_1(uint32_t bits)
    {
        return  ::clear_bank_1(_handle, bits);
    }
    int clear_bank_2( uint32_t bits)
    {
        return ::clear_bank_2(_handle,  bits);
    }
    int set_bank_1(uint32_t bits)
    {
        return ::set_bank_1(_handle, bits);
    }
    int set_bank_2(uint32_t bits)
    {
        return ::set_bank_2(_handle, bits);
    }
    int hardware_clock( unsigned gpio, unsigned clkfreq)
    {
        return ::hardware_clock(_handle,  gpio,  clkfreq);
    }
    int hardware_PWM(unsigned gpio, unsigned PWMfreq, uint32_t PWMduty)
    {
        return ::hardware_PWM(_handle,  gpio, PWMfreq, PWMduty);
    }
    uint32_t get_current_tick()
    {
        return  ::get_current_tick(_handle);
    }
    uint32_t get_hardware_revision()
    {
        return get_hardware_revision();
    }
    uint32_t get_pigpio_version()
    {
        return get_pigpio_version();
    }
    int wave_clear()
    {
        return ::wave_clear(_handle);
    }
    int wave_add_new()
    {
        return ::wave_add_new(_handle);
    }
    int wave_add_generic(unsigned numPulses, gpioPulse_t *pulses)
    {
        return ::wave_add_generic(_handle, numPulses, pulses);
    }
    int wave_add_serial( unsigned user_gpio, unsigned baud, unsigned data_bits,
    unsigned stop_bits, unsigned offset, unsigned numBytes, char *str)
    {
        return ::wave_add_serial(_handle, user_gpio, baud, data_bits, stop_bits, offset,  numBytes, str);
    }
    int wave_create()
    {
        return ::wave_create(_handle);
    }
    int wave_create_and_pad( int percent)
    {
        return ::wave_create_and_pad(_handle, percent);
    }
    int wave_delete(unsigned wave_id)
    {
        return ::wave_delete(_handle, wave_id);
    }
    int wave_send_once(unsigned wave_id)
    {
        return ::wave_send_once(_handle, wave_id);
    }
    int wave_send_repeat(unsigned wave_id)
    {
        return ::wave_send_repeat(_handle, wave_id);
    }
    int wave_send_using_mode(unsigned wave_id, unsigned mode)
    {
        return ::wave_send_using_mode(_handle, wave_id,mode);
    }
    int wave_chain(char *buf, unsigned bufSize)
    {
        return ::wave_chain(_handle, buf, bufSize);
    }
    int wave_tx_at()
    {
        return ::wave_tx_at(_handle);
    }
    int wave_tx_busy()
    {
        return ::wave_tx_busy(_handle);
    }
    int wave_tx_stop()
    {
        return ::wave_tx_stop(_handle);
    }
    int wave_get_micros()
    {
        return ::wave_get_micros(_handle);
    }
    int wave_get_high_micros()
    {
        return ::wave_get_high_micros(_handle);
    }
    int wave_get_max_micros()
    {
        return ::wave_get_max_micros(_handle);
    }
    int wave_get_pulses()
    {
        return ::wave_get_pulses(_handle);
    }
    int wave_get_high_pulses()
    {
        return ::wave_get_high_pulses(_handle);
    }
    int wave_get_max_pulses()
    {
        return ::wave_get_max_pulses(_handle);
    }
    int wave_get_cbs()
    {
        return ::wave_get_cbs(_handle);
    }
    int wave_get_high_cbs()
    {
        return ::wave_get_high_cbs(_handle);
    }
    int gpio_trigger(unsigned user_gpio, unsigned pulseLen, unsigned level)
    {
        return ::gpio_trigger(_handle,  user_gpio,  pulseLen,  level);
    }
    int store_script( char *script)
    {
        return ::store_script(_handle, script);
    }
    int run_script( unsigned script_id, unsigned numPar, uint32_t *param)
    {
        return ::run_script(_handle, script_id,  numPar, param);
    }
    int update_script( unsigned script_id, unsigned numPar, uint32_t *param)
    {
        return ::update_script(_handle, script_id,  numPar, param);
    }
    int script_status(unsigned script_id, uint32_t *param)
    {
        return ::script_status(_handle,  script_id, param);
    }
    int stop_script(unsigned script_id)
    {
        return ::stop_script(_handle, script_id);
    }
    int delete_script(unsigned script_id)
    {
        return ::delete_script(_handle, script_id);
    }
    int bb_serial_read_open( unsigned user_gpio, unsigned baud, unsigned data_bits)
    {
        return ::bb_serial_read_open(_handle, user_gpio,baud, data_bits);
    }
    int bb_serial_read( unsigned user_gpio, void *buf, size_t bufSize)
    {
        return ::bb_serial_read(_handle,  user_gpio, buf,  bufSize);
    }
    int bb_serial_read_close(unsigned user_gpio)
    {
        return ::bb_serial_read_close(_handle, user_gpio);
    }
    int bb_serial_invert(unsigned user_gpio, unsigned invert)
    {
        return ::bb_serial_invert(_handle,  user_gpio,invert);
    }
    int i2c_open(unsigned i2c_bus, unsigned i2c_addr, unsigned i2c_flags)
    {
        return ::i2c_open(_handle,i2c_bus,i2c_addr,i2c_flags);
    }
    int i2c_close(unsigned handle)
    {
        return ::i2c_close(_handle, handle);
    }
    int i2c_write_quick(unsigned handle, unsigned bit)
    {
        return ::i2c_write_quick(_handle,  handle, bit);
    }
    int i2c_write_byte( unsigned handle, unsigned bVal)
    {
        return ::i2c_write_byte(_handle, handle, bVal);
    }
    int i2c_read_byte(unsigned handle)
    {
        return ::i2c_read_byte(_handle, handle);
    }
    int i2c_write_byte_data(unsigned handle, unsigned i2c_reg, unsigned bVal)
    {
        return ::i2c_write_byte_data(_handle,  handle,  i2c_reg, bVal);
    }
    int i2c_write_word_data( unsigned handle, unsigned i2c_reg, unsigned wVal)
    {
        return ::i2c_write_word_data( _handle, handle, i2c_reg, wVal);
    }
    int i2c_read_byte_data( unsigned handle, unsigned i2c_reg)
    {
        return ::i2c_read_byte_data(_handle,  handle,  i2c_reg);
    }
    int i2c_read_word_data(unsigned handle, unsigned i2c_reg)
    {
        return ::i2c_read_word_data(_handle, handle, i2c_reg);
    }
    int i2c_process_call(unsigned handle, unsigned i2c_reg, unsigned wVal)
    {
        return ::i2c_process_call(_handle, handle,i2c_reg, wVal);
    }
    int i2c_write_block_data( unsigned handle, unsigned i2c_reg, char *buf, unsigned count)
    {
        return ::i2c_write_block_data(_handle,  handle,  i2c_reg, buf,  count);
    }
    int i2c_read_block_data( unsigned handle, unsigned i2c_reg, char *buf)
    {
        return ::i2c_read_block_data(_handle, handle, i2c_reg, buf);
    }
    int i2c_block_process_call(  unsigned handle, unsigned i2c_reg, char *buf, unsigned count)
    {
        return ::i2c_block_process_call( _handle,  handle,  i2c_reg, buf,  count);
    }
    int i2c_read_i2c_block_data(  unsigned handle, unsigned i2c_reg, char *buf, unsigned count)
    {
        return ::i2c_read_i2c_block_data( _handle,  handle,  i2c_reg, buf, count);
    }
    int i2c_write_i2c_block_data( unsigned handle, unsigned i2c_reg, char *buf, unsigned count)
    {
        return ::i2c_write_i2c_block_data(_handle, handle, i2c_reg, buf, count);
    }
    int i2c_read_device(unsigned handle, char *buf, unsigned count)
    {
        return ::i2c_read_device(_handle, handle, buf, count);
    }
    int i2c_write_device( unsigned handle, char *buf, unsigned count)
    {
        return ::i2c_write_device(_handle, handle, buf, count);
    }
    int i2c_zip( unsigned handle,        char    *inBuf,        unsigned inLen,        char    *outBuf,        unsigned outLen)
    {
        return ::i2c_zip(_handle,  handle,  inBuf,     inLen,    outBuf,    outLen);
    }
    int bb_i2c_open( unsigned SDA, unsigned SCL, unsigned baud)
    {
        return ::bb_i2c_open(_handle, SDA, SCL, baud);
    }
    int bb_i2c_close( unsigned SDA)
    {
        return ::bb_i2c_close(_handle,  SDA);
    }
    int bb_i2c_zip(        unsigned SDA,        char    *inBuf,        unsigned inLen,        char    *outBuf,        unsigned outLen)
    {
        return ::bb_i2c_zip(    _handle,    SDA,    inBuf,    inLen,    outBuf,     outLen);
    }
    int bb_spi_open(unsigned CS, unsigned MISO, unsigned MOSI, unsigned SCLK, unsigned baud, unsigned spi_flags)
    {
        return  ::bb_spi_open(_handle, CS, MISO, MOSI, SCLK,baud, spi_flags);
    }
    int bb_spi_close(unsigned CS)
    {
        return ::bb_spi_close(_handle,CS);
    }
    int bb_spi_xfer(unsigned CS,char    *txBuf,char    *rxBuf, unsigned count)
    {
        return ::bb_spi_xfer( _handle, CS, txBuf, rxBuf, count);
    }
    int spi_open(unsigned spi_channel, unsigned baud, unsigned spi_flags)
    {
        return ::spi_open(_handle, spi_channel, baud, spi_flags);
    }
    int spi_close(unsigned handle)
    {
        return ::spi_close(_handle,  handle);
    }
    int spi_read(unsigned handle, char *buf, unsigned count)
    {
        return ::spi_read(_handle,handle, buf, count);
    }
    int spi_write(unsigned handle, char *buf, unsigned count)
    {
        return ::spi_write(_handle, handle, buf, count);
    }
    int spi_xfer( unsigned handle, char *txBuf, char *rxBuf, unsigned count)
    {
        return ::spi_xfer(_handle,  handle, txBuf, rxBuf, count);
    }
    int serial_open( char *ser_tty, unsigned baud, unsigned ser_flags)
    {
        return ::serial_open(_handle, ser_tty, baud, ser_flags);
    }
    int serial_close( unsigned handle)
    {
        return ::serial_close(_handle, handle);
    }
    int serial_write_byte(unsigned handle, unsigned bVal)
    {
        return ::serial_write_byte(_handle,  handle,  bVal);
    }
    int serial_read_byte( unsigned handle)
    {
        return ::serial_read_byte(_handle, handle);
    }
    int serial_write( unsigned handle, char *buf, unsigned count)
    {
        return ::serial_write(_handle, handle, buf, count);
    }
    int serial_read( unsigned handle, char *buf, unsigned count)
    {
        return ::serial_read(_handle,  handle, buf,  count);
    }
    int serial_data_available( unsigned handle)
    {
            return ::serial_data_available(_handle,handle);
    }
    int custom_1(unsigned arg1, unsigned arg2, char *argx, unsigned argc)
    {
        return ::custom_1(_handle,  arg1, arg2, argx, argc);
    }
    int custom_2(unsigned arg1, char *argx, unsigned argc, char *retBuf, unsigned retMax)
    {
        return ::custom_2(_handle, arg1, argx, argc, retBuf, retMax);
    }
    int get_pad_strength(unsigned pad)
    {
        return ::get_pad_strength(_handle, pad);
    }
    int set_pad_strength( unsigned pad, unsigned padStrength)
    {
        return ::set_pad_strength(_handle,  pad, padStrength);
    }
    int shell_( char *scriptName, char *scriptString)
    {
        return ::shell_(_handle, scriptName, scriptString);
    }
    int file_open( char *file, unsigned mode)
    {
        return ::file_open(_handle, file,  mode);
    }
    int file_close( unsigned handle)
    {
        return ::file_close(_handle,  handle);
    }
    int file_write( unsigned handle, char *buf, unsigned count)
    {
        return ::file_write(_handle, handle, buf, count);
    }
    int file_read( unsigned handle, char *buf, unsigned count)
    {
        return ::file_read(_handle, handle, buf, count);
    }
    int file_seek(unsigned handle, int32_t seekOffset, int seekFrom)
    {
        return ::file_seek(_handle,  handle,  seekOffset,  seekFrom);
    }
    int file_list( char *fpat,  char *buf, unsigned count)
    {
        return ::file_list(_handle, fpat,  buf,  count);
    }
    int callback( unsigned user_gpio, unsigned edge, CBFunc_t f)
    {
        return ::callback(_handle,  user_gpio,  edge,  f);
    }
    int callback_ex( unsigned user_gpio, unsigned edge, CBFuncEx_t f, void *userdata)
    {
        return ::callback_ex(_handle,  user_gpio,  edge,  f, userdata);
    }
    int callback_cancel(unsigned callback_id)
    {
        return ::callback_cancel( callback_id);
    }
    int wait_for_edge(unsigned user_gpio, unsigned edge, double timeout)
    {
        return ::wait_for_edge(_handle,  user_gpio, edge,  timeout);
    }
    int bsc_xfer( bsc_xfer_t *bscxfer)
    {
        return ::bsc_xfer(_handle, bscxfer);
    }
    int bsc_i2c( int i2c_addr, bsc_xfer_t *bscxfer)
    {
        return ::bsc_i2c(_handle,  i2c_addr, bscxfer);
    }
    int event_callback( unsigned event, evtCBFunc_t f)
    {
        return ::event_callback(_handle,  event,  f);
    }
    int event_callback_ex( unsigned event, evtCBFuncEx_t f, void *userdata)
    {
        return ::event_callback_ex(_handle,  event,  f, userdata);
    }
    int event_callback_cancel(unsigned callback_id)
    {
        return ::event_callback_cancel(callback_id);
    }
    int  wait_for_event( unsigned event, double timeout)
    {
        return ::wait_for_event(_handle, event,  timeout);
    }
    int event_trigger( unsigned event)
    {
        return ::event_trigger(_handle,  event);
    }

};

}
#endif // PIGPIO_H
