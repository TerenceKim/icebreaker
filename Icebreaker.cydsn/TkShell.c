/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <string.h>
#include <Application.h>
#include <ctype.h>
#include "TkShell.h"
#include <Codec.h>
#include <VolumeControl.h>
#include <Calibration.h>
#include <LED.h>
#include <USBUART_cdc.h>
#include <timers.h>
#include <LedManager.h>
#include <AudioManager.h>
#include <cc85xx.h>

#define USBUART_BUFFER_SIZE (64u)

#define TK_SHELL_METHOD(c, verb)              int __tk_shell_ ## c ## _ ## verb(int __unused argc, char __unused **argv)
#define TK_SHELL_COMMAND(name, desc)          {#name, (tk_shell_command_verb_s *)__tk_shell_verbs_ ## name, desc}
#define TK_SHELL_VERBS(name)                  const tk_shell_command_verb_s __tk_shell_verbs_ ## name[] 
#define TK_SHELL_VERB(c, name, desc)          {#name, __tk_shell_ ## c ## _ ## name, desc}

#define CMD_BUF_LEN 64
#define TK_SHELL_MAX_ARGS 10

typedef struct
{
  const char *name;
  int        (*func)(int argc, char **argv);
  const char *desc;
} tk_shell_command_verb_s;

typedef struct
{
  const char *name;
  tk_shell_command_verb_s *verbs;
  const char *desc;
} tk_shell_command_s;


static const uint32_t reg_lookup[] = 
{
    CYREG_PRT0_DR,  // 0
    CYREG_PRT1_DR,  // 1
    CYREG_PRT2_DR,  // 2
    CYREG_PRT3_DR,  // 3
    CYREG_PRT4_DR,  // 4
    CYREG_PRT5_DR,  // 5
    CYREG_PRT6_DR,  // 6
    CYREG_PRT7_DR,  // 7
    CYREG_PRT8_DR,  // 8
    CYREG_PRT9_DR,  // 9
    CYREG_PRT10_DR, // 10
    CYREG_PRT11_DR, // 11
    CYREG_PRT12_DR, // 12
    CYREG_PRT13_DR, // 13
};
TK_SHELL_METHOD(gpio, set);
TK_SHELL_METHOD(gpio, config);
TK_SHELL_METHOD(gpio, get);
static TK_SHELL_VERBS(gpio) =
{
    TK_SHELL_VERB(gpio, set, "set GPIO state"),
    TK_SHELL_VERB(gpio, config, "configure GPIO"),
    TK_SHELL_VERB(gpio, get, "get GPIO state"),
    { "", NULL, "" }
};

TK_SHELL_METHOD(wm, vol_set);
TK_SHELL_METHOD(wm, vol_get);
TK_SHELL_METHOD(wm, reg_wr);
TK_SHELL_METHOD(wm, reg_rd);
static TK_SHELL_VERBS(wm) =
{
    TK_SHELL_VERB(wm, vol_set, "set volume level <vol:uint8>"),
    TK_SHELL_VERB(wm, vol_get, "get volume level"),
    TK_SHELL_VERB(wm, reg_wr, "write register <reg:uint8,data:uint16>)"),
    TK_SHELL_VERB(wm, reg_rd, "read register <reg:uint8>"),
    { "", NULL, "" }
};

TK_SHELL_METHOD(rf, init);
TK_SHELL_METHOD(rf, reset);
TK_SHELL_METHOD(rf, bl_reset);
TK_SHELL_METHOD(rf, bl_unlock);
TK_SHELL_METHOD(rf, bl_erase);
TK_SHELL_METHOD(rf, bl_verify);
TK_SHELL_METHOD(rf, bl_flash);
TK_SHELL_METHOD(rf, status);
TK_SHELL_METHOD(rf, info);
static TK_SHELL_VERBS(rf) =
{
  TK_SHELL_VERB(rf, init, "initialize RF chip"),
  TK_SHELL_VERB(rf, reset, "perform sys reset"),
  TK_SHELL_VERB(rf, bl_reset, "perform boot reset"),
  TK_SHELL_VERB(rf, bl_unlock, "unlock bootloader SPI"),
  TK_SHELL_VERB(rf, bl_erase, "perform mass erase"),
  TK_SHELL_VERB(rf, bl_verify, "perform image verification"),
  TK_SHELL_VERB(rf, bl_flash, "flash a chunk: <addr> <data:uint8[]>"),
  TK_SHELL_VERB(rf, status, "get status"),
  TK_SHELL_VERB(rf, info, "print dev_chip_info"),
  { "", NULL, "" }
};

TK_SHELL_METHOD(sys, crash);
TK_SHELL_METHOD(sys, info);
TK_SHELL_METHOD(sys, reset);
TK_SHELL_METHOD(sys, adc);
static TK_SHELL_VERBS(sys) = 
{
    TK_SHELL_VERB(sys, crash, "trigger a hard fault"),
    TK_SHELL_VERB(sys, info, "print info"),
    TK_SHELL_VERB(sys, reset, "trigger a SW reset"),
    TK_SHELL_VERB(sys, adc, "print ADC values"),
    { "", NULL, "" }
};

TK_SHELL_METHOD(led, pwm_period);
TK_SHELL_METHOD(led, start);
TK_SHELL_METHOD(led, stop);
static TK_SHELL_VERBS(led) =
{
    TK_SHELL_VERB(led, start, "start LED animation <led:red|green|blue> <period:uint32> <compare:uint32> <counter:uint32>"),
    TK_SHELL_VERB(led, stop, "stop LED animation <led:red|green|blue>"),
    { "", NULL, "" }
};

TK_SHELL_METHOD(cal, print);
TK_SHELL_METHOD(cal, set);
TK_SHELL_METHOD(cal, save);
static TK_SHELL_VERBS(cal) =
{
    TK_SHELL_VERB(cal, print, "print cal data"),
    TK_SHELL_VERB(cal, set, "set cal data"),
    TK_SHELL_VERB(cal, save, "commit cal data to flash"),
    { "", NULL, "" }
};

TK_SHELL_METHOD(audio, play_tone);
TK_SHELL_METHOD(audio, stop_tone);
static TK_SHELL_VERBS(audio) =
{
  TK_SHELL_VERB(audio, play_tone, "play tone"),
  TK_SHELL_VERB(audio, stop_tone, "stop tone"),
  { "", NULL, "" }
};

static const tk_shell_command_s commands[] = 
{
    TK_SHELL_COMMAND(gpio, "GPIO commands"),
    TK_SHELL_COMMAND(wm, "Wolfson Codec commands"),
    TK_SHELL_COMMAND(rf, "RF chip commands"),
    TK_SHELL_COMMAND(sys, "System commands"),
    TK_SHELL_COMMAND(led, "LED commands"),
    TK_SHELL_COMMAND(cal, "Calibration data commands"),
    TK_SHELL_COMMAND(audio, "Audio commands"),
    { "", NULL, "" }
};

static const char OK_STR[] = "ok";
static const char ER_STR[] = "er";

static char cmd_buf[CMD_BUF_LEN];
static uint8_t cmd_char_count;

////////////////////////////////////////////////////////////////////////////////
int strcicmp(char const *a, char const *b)
{
    for (;; a++, b++)
    {
        int d = tolower(*a) - tolower(*b);
        if (d != 0 || !*a)
            return d;
    }
}

TK_SHELL_METHOD(gpio, set)
{
    uint32_t port, pin, state;
    int i = 2;
    
    argc -= i;

    if (argc != 3)
    {
        PRINTF("Invalid number of arguments!\n");
        return -1;
    }
    
    port = atoi(argv[i++]);
    pin = atoi(argv[i++]);
    state = atoi(argv[i]);
    
    CY_SYS_PINS_SET_DRIVE_MODE(reg_lookup[port], pin, CY_SYS_PINS_DM_STRONG);

    if (state)
    {
        CY_SYS_PINS_SET_PIN(reg_lookup[port], pin);
    }
    else
    {
        CY_SYS_PINS_CLEAR_PIN(reg_lookup[port], pin);
    }
    
    PRINTF("> gpio:ok %d\n", (int)CY_SYS_PINS_READ_PIN(reg_lookup[port], pin));
    return 0;
}

TK_SHELL_METHOD(gpio, config)
{
    uint32_t port, pin, mode;
    int i = 2;
    
    argc -= i;

    if (argc != 3)
    {
        PRINTF("Invalid number of arguments!\n");
        return -1;
    }
    
    port = atoi(argv[i++]);
    pin = atoi(argv[i++]);
    mode = atoi(argv[i]);
    
    CY_SYS_PINS_SET_DRIVE_MODE(reg_lookup[port], pin, mode);
    
    return 0;
}

TK_SHELL_METHOD(gpio, get)
{
    uint32_t port, pin;
    int i = 2;
    
    argc -= i;

    if (argc != 2)
    {
        PRINTF("Invalid number of arguments!\n");
        return -1;
    }
    
    port = atoi(argv[i++]);
    pin = atoi(argv[i]);
    
    PRINTF("> gpio:ok %d\n", (int)CY_SYS_PINS_READ_PIN(reg_lookup[port], pin));
    
    return 0;
}

TK_SHELL_METHOD(wm, vol_set)
{
    int vol;
    int i = 2;
    uint8_t rv;
    
    argc -= i;
    
    if (argc != 1)
    {
        PRINTF("Invalid number of arguments!\n");
        return -1;
    }

    vol = atoi(argv[i++]);

    if ((rv = Codec_AdjustBothHeadphoneVolume(vol)) == 0)
    {
        PRINTF("> wm:ok\n");
    }
    else
    {
        PRINTF("> wm:er %d\n", rv);
        return -2;
    }

    return 0;
}

TK_SHELL_METHOD(wm, vol_get)
{
    PRINTF("> wm:ok %d\n", (int)Codec_GetHeadphoneVolume());

    return 0;
}

TK_SHELL_METHOD(wm, reg_wr)
{
    int i = 2;
    uint8_t reg;
    uint16_t data;
    int rv;
    
    argc -= i;
    
    if (argc != 2)
    {
        PRINTF("Invalid number of arguments!\n");
        return -1;
    }
    
    reg = atoi(argv[i++]);
    data = strtoul(argv[i++], NULL, 16);
    
    if ((rv = Codec_SendData(reg, data)) == 0)
    {
        PRINTF("> wm:ok\n");
    }
    else
    {
        PRINTF("> wm:er %d\n", rv);
        return -2;
    }
    
    return 0;
}

TK_SHELL_METHOD(wm, reg_rd)
{
    int i = 2;
    uint8_t reg;
    
    argc -= i;
    
    if (argc != 1)
    {
        PRINTF("Invalid number of arguments!\n");
        return -1;
    }
    
    reg = atoi(argv[i++]);
    
    PRINTF("> wm:ok 0x%04X\n", Codec_GetData(reg));
    
    return 0;
}

TK_SHELL_METHOD(rf, init)
{
  cc85xx_init();
  PRINTF("> rf:ok\n");
  return 0;
}

TK_SHELL_METHOD(rf, reset)
{
  PRINTF("> rf:%s\n", (cc85xx_sys_reset()) ? OK_STR : ER_STR);
  return 0;
}

TK_SHELL_METHOD(rf, bl_reset)
{
  PRINTF("> rf:%s\n", (cc85xx_boot_reset()) ? OK_STR : ER_STR);
  return 0;
}

TK_SHELL_METHOD(rf, bl_unlock)
{
  PRINTF("> rf:%s\n", (cc85xx_bl_unlock_spi()) ? OK_STR : ER_STR);
  return 0;
}

TK_SHELL_METHOD(rf, bl_erase)
{
  PRINTF("> rf:%s\n", (cc85xx_bl_mass_erase()) ? OK_STR : ER_STR);
  return 0;
}

TK_SHELL_METHOD(rf, bl_verify)
{
  uint32_t crc;
  bool ret = cc85xx_bl_flash_verify(&crc);

  PRINTF("> rf:%s 0x%08X\n", (ret) ? OK_STR : ER_STR, crc);

  return 0;
}

TK_SHELL_METHOD(rf, bl_flash)
{
  int i = 2, j;
  uint16_t addr;
  uint8_t data[CMD_BUF_LEN];
  char hex[3];
  
  argc -= i;
  
  if (argc != 2)
  {
      PRINTF("Invalid number of arguments: %d\n", argc);
      return -1;
  }
  
  addr = strtoul(argv[i++], NULL, 16);
  
  for (j = 0; j < (int)strlen(argv[i]); j += 2)
  {
    hex[0] = argv[i][j];
    hex[1] = argv[i][j + 1];
    hex[2] = '\0';
    data[j/2] = strtoul(hex, NULL, 16);
  }
  
  PRINTF("> rf:%s\n", (cc85xx_flash_bytes(addr, data, j/2)) ? OK_STR : ER_STR);
  return 0;
}

TK_SHELL_METHOD(rf, status)
{
  PRINTF("> rf:ok 0x%04X\n", cc85xx_get_status());
  return 0;
}

TK_SHELL_METHOD(rf, info)
{
  PRINTF("> rf:%s\n", (cc85xx_print_info()) ? OK_STR : ER_STR);
  return 0;
}

TK_SHELL_METHOD(sys, crash)
{
    void (*fp)(void) = NULL;
    
    fp();
    
    PRINTF("> sys:er\n");
    
    return 0;
}

TK_SHELL_METHOD(sys, info)
{
    PRINTF("> sys:ok %08lu\n", (unsigned long)tmrGetCounter_ms());

    return 0;
}

TK_SHELL_METHOD(sys, reset)
{
    CySoftwareReset();
    
    return 0;
}

TK_SHELL_METHOD(sys, adc)
{
    PRINTF("> sys:ok %d,%d\n", adcSample[vol_ctrl_left], adcSample[vol_ctrl_right]);
    
    return 0;
}

TK_SHELL_METHOD(led, start)
{
    int i = 2;
    LM_LED_e ledIdx = LM_LED_MAX;
    uint32_t pwmPeriod, pwmCompare, pwmCounter;
    
    argc -= i;
    
    if (argc != 4)
    {
        PRINTF("Invalid number of arguments: %d\n", argc);
        return -1;
    }
    
    if (strcicmp(argv[i], "red") == 0)
    {
        ledIdx = LM_LED_RED;
    }
    else if (strcicmp(argv[i], "green") == 0)
    {
        ledIdx = LM_LED_GREEN;
    }
    else if (strcicmp(argv[i], "blue") == 0)
    {
        ledIdx = LM_LED_BLUE;
    }
    else
    {
        PRINTF("Invalid color %s, must be red|green|blue\n", argv[i]);
        return -2;
    }
    
    pwmPeriod = atoi(argv[i + 1]);
    pwmCompare = atoi(argv[i + 2]);
    pwmCounter = atoi(argv[i + 3]);
    
    LedManagerStartOverride(ledIdx, pwmPeriod, pwmCompare, pwmCounter);
    
    PRINTF("> led:ok\n");

    return 0;
}

TK_SHELL_METHOD(led, stop)
{
    int i = 2;
    LM_LED_e ledIdx = LM_LED_MAX;
    
    argc -= i;
    
    if (argc != 1)
    {
        PRINTF("Invalid number of arguments: %d\n", argc);
        return -1;
    }
    
    if (strcicmp(argv[i], "red") == 0)
    {
        ledIdx = LM_LED_RED;
    }
    else if (strcicmp(argv[i], "green") == 0)
    {
        ledIdx = LM_LED_GREEN;
    }
    else if (strcicmp(argv[i], "blue") == 0)
    {
        ledIdx = LM_LED_BLUE;
    }
    else
    {
        PRINTF("Invalid color %s, must be red|green|blue\n", argv[i]);
        return -2;
    }
    LedManagerStopOverride(ledIdx);
    
    PRINTF("> led:ok\n");

    return 0;
}

TK_SHELL_METHOD(cal, print)
{
    PRINTF("sign = 0x%08X\n", (unsigned int)cal_data->signature);
    PRINTF("ctr = %lu\n", cal_data->counter);
    PRINTF("adc_min[R] = %d\n", cal_data->adc_min[vol_ctrl_right]);
    PRINTF("adc_max[R] = %d\n", cal_data->adc_max[vol_ctrl_right]);
    PRINTF("adc_min[L] = %d\n", cal_data->adc_min[vol_ctrl_left]);
    PRINTF("adc_max[L] = %d\n", cal_data->adc_max[vol_ctrl_left]);

    PRINTF("led_max_brightness = %d\n", cal_data->led_max_brightness);
    PRINTF("debug_mask = 0x%02X\n", (unsigned int)cal_data->debug_mask);
    PRINTF("> cal:ok\n");

    return 0;    
}

TK_SHELL_METHOD(cal, set)
{
    int i = 2;
    
    argc -= i;
    
    if (argc == 0)
    {
        PRINTF("Invalid number of arguments!\n");
        return -1;
    }
    
    if (strcicmp(argv[i], "led_max_brightness") == 0)
    {
        cal_data->led_max_brightness = atoi(argv[++i]);
    }
    else if (strcicmp(argv[i], "adc_min_l") == 0)
    {
        cal_data->adc_min[vol_ctrl_left] = atoi(argv[++i]);
    }
    else if (strcicmp(argv[i], "debug_mask") == 0)
    {
        cal_data->debug_mask = strtoul(argv[++i], NULL, 16);
    }
    return 0;
}

TK_SHELL_METHOD(cal, save)
{
    PRINTF("> cal:ok %d\n", CalibrationSave());
    return 0;
}

TK_SHELL_METHOD(audio, play_tone)
{
  AudioManagerToneStart(strtoul(argv[2], NULL, 16));
  
  PRINTF("> audio:ok\n");

  return 0;
}

TK_SHELL_METHOD(audio, stop_tone)
{
  AudioManagerToneStop();
  PRINTF("> audio:ok\n");
  return 0;
}

static int TkShellProcessCommand(void)
{
    int retval = -1;
    char *buf = cmd_buf;
    bool found = false;
    char *argv[TK_SHELL_MAX_ARGS];
    int argc = 0;
    int i = 0, j = 0, k = 0;
    
    while (1)
    {
        // skip over whitespace
        while (*buf && isspace(*buf))
        {
          *buf = '\0';
          ++buf;
        }

        if (*buf == '\0')
        {
          break;
        }
        
        argv[argc] = buf;
        
        // skip over the rest of arg until whitespace again
        while (*buf && !isspace(*buf))
        {
            buf++;
        }
        ++argc;
        
        if (argc > TK_SHELL_MAX_ARGS)
        {
            PRINTF("Too many arguments!\n");
            return -2;
        }
    }
    
    if (argc > 0)
    {
        i = j = k = 0;
        while (strcmp(commands[i].name, "") != 0)
        {
            if (strcicmp(argv[0], commands[i].name) == 0)
            {
                while (strcmp(commands[i].verbs[j].name, "") != 0)
                {
                    if (strcicmp(argv[1], commands[i].verbs[j].name) == 0)
                    {
                        retval = commands[i].verbs[j].func(argc, argv);
                        found = true;
                        break;
                    }
                    else if (strcicmp(argv[1], "help") == 0)
                    {
                        found = true;
                        PRINTF("Displaying list of %s commands:\n", commands[i].name);
                        while (strcmp(commands[i].verbs[k].name, "") != 0)
                        {
                            PRINTF("\t%s %s: %s\n", commands[i].name, commands[i].verbs[k].name, commands[i].verbs[k].desc);
                            k++;
                        }
                        break;
                    }
                    j++;
                }

                if (found)
                {
                    break;
                }
            }
            else if (strcicmp(argv[0], "help") == 0)
            {
                found = true;
                PRINTF("Displaying list of commands:\n");
                while (strcmp(commands[j].name, "") != 0)
                {
                    PRINTF("\t%s: %s\n", commands[j].name, commands[j].desc);
                    j++;
                }
                break;
            }
            i++;
        }
    }
    
    if (!found)
    {
        PRINTF("unknown command: %s\n", argv[0]);
    }
    
    return retval;
}

void TkShellInit(void)
{
    memset((void *)cmd_buf, 0, CMD_BUF_LEN);
    cmd_char_count = 0;
}

void TkShellService(void)
{
    char c;
    uint8 i = 0;
    uint16 count;
    uint8 buffer[USBUART_BUFFER_SIZE];
//    
    /* Read received data and re-enable OUT endpoint. */
    count = USBUART_GetAll(buffer);
//
//                if (0u != count)
//                {
//                    /* Wait until component is ready to send data to host. */
//                    while (0u == USBUART_CDCIsReady())
//                    {
//                    }
//
//                    /* Send data back to host. */
//                    USBUART_PutData(buffer, count);
//
//                    /* If the last sent packet is exactly the maximum packet 
//                    *  size, it is followed by a zero-length packet to assure
//                    *  that the end of the segment is properly identified by 
//                    *  the terminal.
//                    */
//                    if (USBUART_BUFFER_SIZE == count)
//                    {
//                        /* Wait until component is ready to send data to PC. */
//                        while (0u == USBUART_CDCIsReady())
//                        {
//                        }
//
//                        /* Send zero-length packet to PC. */
//                        USBUART_PutData(NULL, 0u);
//                    }
//                }
//
    while ((c = buffer[i]) != 0 && i < count)
    {
        i++;
        switch (c)
        {
            case '\r':
            case '\n':
            case 0x03:
                PRINTF("%c", c);
                if (cmd_char_count > 0)
                {
                    TkShellProcessCommand();
                }
                PRINTF("\U00002744 ] ");
                cmd_char_count = 0;
                cmd_buf[0] = '\0';
                break;
                
            case 127:
            case '\b':
                if (cmd_char_count == 0)
                {
                    break;
                }
                cmd_buf[--cmd_char_count] = '\0';
                PRINTF("\b \b");
                break;

            default:
                PRINTF("%c", c);
                cmd_buf[cmd_char_count++] = c;
                cmd_buf[cmd_char_count] = '\0';
                break;
        }
    }
}
/* [] END OF FILE */
