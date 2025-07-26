                //Librerias
      
#include <Q2HX711.h>                            //Librería HX711
#include <EEPROM.h>                             //Librería EEPROM
#include "Nextion.h"                            //Librería Nextion
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //Pines Arduino
const byte hx711_data_pin = 3;    //Data pin from HX711
const byte hx711_clock_pin = 2;   //Clock pin from HX711
Q2HX711 hx711(hx711_data_pin, hx711_clock_pin); // prep hx71
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////      
                    //Declaración variables generales
long x1 = 0L;
long x0 = 0L;
long x1a = 0L;
long x0a = 0L;
float avg_size = 2;       //4y,.0; // cantidad de promedios para cada medición de masa

int peso = 0;
int peso_dosi = 0;
int peso_dosi_visu = 0;
int peso_llen_dosi = 0;
int peso_vuel_dosi = 0;

int dire1a = 0;
int dire1b = 0;
int dire1c = 0;
int dire2a = 4;
int dire2b = 4;
int dire2c = 4;
int dire2d = 4;
int dire3a = 10;
int dire3b = 10;
int dire4a = 14;
int dire4b = 14;
int dire5a = 20;
int dire5b = 20;
int dire5c = 20;
int dire5d = 20;


int ok_capt_adc;
int ok_cali_peso;

int moto = 7;
int peda = 9;
int valo_peda = 0;
bool dosi_auto = false;
int inic_dosi = 0;

int esta_cali = 0;

uint32_t peso_cono;
uint32_t peso_cono_r;
uint32_t peso_cono_pesa;
uint32_t peso_llen_r;
uint32_t peso_llen;
uint32_t peso_vuel_r;
uint32_t peso_vuel;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //NEXTION
                    //Declaración variables principal
NexNumber peso_visu = NexNumber(1, 5, "peso_visu");                    // Declara un objeto de la clase NexNumber llamado peso_visu.
NexNumber visu_peso_llen = NexNumber(1, 12, "visu_peso_llen");         // Declara un objeto de la clase NexNumber llamado visu_peso_llen.
NexNumber visu_peso_dosi = NexNumber(1, 8, "visu_peso_dosi");          // Declara un objeto de la clase NexNumber llamado visu_peso_dosi.
                    //Declaración variables peso a llenar
NexNumber lect_peso_llen = NexNumber(2, 4, "lect_peso_llen");          // Declara un objeto de la clase NexNumber llamado lect_peso_llen.
NexNumber escr_peso_llen = NexNumber(2, 7, "escr_peso_llen");          // Declara un objeto de la clase NexNumber llamado escr_peso_llen.
NexButton capt_peso_llen = NexButton(2, 9, "capt_peso_llen");          // Declara un objeto de la clase NexButton llamado capt_peso_llen.
NexNumber lect_peso_vuel = NexNumber(2, 11, "lect_peso_vuel");         // Declara un objeto de la clase NexNumber llamado lect_peso_vuel.
NexNumber escr_peso_vuel = NexNumber(2, 14, "escr_peso_vuel");         // Declara un objeto de la clase NexNumber llamado escr_peso_vuel.
NexButton capt_peso_vuel = NexButton(2, 16, "capt_peso_vuel");         // Declara un objeto de la clase NexButton llamado capt_peso_vuel.
                    //Declaración variables calibración
NexNumber lect_peso_cono = NexNumber(3, 4, "lect_peso_cono");          // Declara un objeto de la clase NexNumber llamado lect_peso_cono.
NexNumber escr_peso_cono = NexNumber(3, 6, "escr_peso_cono");          // Declara un objeto de la clase NexNumber llamado escr_peso_cono.
NexButton capt_peso_cono = NexButton(3, 8, "capt_peso_cono");          // Declara un objeto de la clase NexButton llamado capt_peso_cono.
NexButton cali_peso = NexButton(3, 9, "cali_peso");                    // Declara un objeto de la clase NexButton llamado cali_peso.
NexText visu_esta_cali = NexText(3, 13, "visu_esta_cali");             // Declara un objeto de la clase NexText llamado visu_esta_cali.
                    // Lista de eventos táctiles de la nextion.  
NexTouch *nex_listen_list[] =
{
                    //Lista de objetos táctiles de la nextion.  
  &peso_visu,
  &visu_peso_llen,
  &visu_peso_dosi,
  &lect_peso_llen,
  &escr_peso_llen,
  &capt_peso_llen,
  &lect_peso_vuel,
  &escr_peso_vuel,
  &capt_peso_vuel,
  &lect_peso_cono,
  &escr_peso_cono,
  &capt_peso_cono,
  &cali_peso,
  &visu_esta_cali,
                
  NULL
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    //Funcion de llamda componenetes numericos principal
                    //Captura de valores numericos calibración
void capt_peso_conoPopCallback(void *ptr)                
{
  dbSerialPrintln("capt_peso_conoPopCallback");
  escr_peso_cono.getValue(&peso_cono_r);
  EEPROM.put(dire1a, peso_cono_r);
                    //Lectura de memoria 0 - escritura a Nextion peso configuración
  EEPROM.get(dire1b, peso_cono);
  lect_peso_cono.setValue(peso_cono);
}
                    //Captura de valores numericos peso a llenar
void capt_peso_llenPopCallback(void *ptr)                
{
  dbSerialPrintln("capt_peso_llenPopCallback");
  escr_peso_llen.getValue(&peso_llen_r);
  EEPROM.put(dire2a, peso_llen_r);
                    //Lectura de memoria 4 - escritura a Nextion peso a llenar    
  EEPROM.get(dire2b, peso_llen);    
  lect_peso_llen.setValue(peso_llen);
  visu_peso_llen.setValue(peso_llen);
}
                    //Captura de valores numericos peso en vuelo
void capt_peso_vuelPopCallback(void *ptr)                
{
  dbSerialPrintln("capt_peso_vuelPopCallback");
  escr_peso_vuel.getValue(&peso_vuel_r);
  EEPROM.put(dire5a, peso_vuel_r);
                    //Lectura de memoria 5 - escritura a Nextion peso en vuelo
  EEPROM.get(dire5b, peso_vuel);    
  lect_peso_vuel.setValue(peso_vuel);
}
                    //
void cali_pesoPopCallback(void *ptr)                     
{
    dbSerialPrintln("cali_pesoPopCallback");
    esta_cali = 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
                    //NEXTION
  nexInit();                                        // Inicia comunicacion con la pantalla Nextion.
                    //
  capt_peso_llen.attachPop(capt_peso_llenPopCallback);                // Registra la llamada funcion denerada por el capt_peso_llen.
  capt_peso_vuel.attachPop(capt_peso_vuelPopCallback);               // Registra la llamada funcion denerada por el capt_peso_vuel.
  capt_peso_cono.attachPop(capt_peso_conoPopCallback);                // Registra la llamada funcion denerada por el capt_peso_cono.
  cali_peso.attachPop(cali_pesoPopCallback);                          // Registra la llamada funcion denerada por el cali_peso.

                    //
  dbSerialPrintln("setup done");             
                    //Inicio de entradas - salidas digitales
  pinMode(moto, OUTPUT);
  pinMode(peda, INPUT);
 digitalWrite(moto, HIGH);
                    //Inicio de puerto de comunicacion
  Serial.begin(9600);    
                   //Habilitar escaneo PCMSK0 
  PCICR |= (1 << PCIE0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
                      // Rutina de escucha eventos producidos en la pantalla.
  nexLoop(nex_listen_list); 
                      //Definicion de pines
  valo_peda = digitalRead(peda);                //Lectura digital de entrada
                      //Procedimiento de calibracion de bascula
  if (esta_cali==1)
  { 
    visu_esta_cali.setText("INICIO CALIBRACION");          
                      //
    delay(2000);                        // permita que la celda de carga y el hx711 se asienten
  
                      // Procedimiento de tara
    for (int ii=0;ii<int(avg_size);ii++)
    {
      delay(10);
      x0+=hx711.read();
    }
    x0/=long(avg_size);
    EEPROM.put(dire3a, x0);
    int ii = 1;
    while(true)
    {
      if (hx711.read()<x0+10000)
      {
                      //hacer nada...
        visu_esta_cali.setText("COLOCAR PESO CONOCIDO");
      }
      else 
      {
        ii++;
        delay(2000);
        for (int jj=0;jj<int(avg_size);jj++)
        {
          x1+=hx711.read();
        }
        x1/=long(avg_size);
        EEPROM.put(dire4a, x1);
        break;
      }
    }
    delay (2000);
    visu_esta_cali.setText("FIN CALIBRACION");
    delay (1000);
    visu_esta_cali.setText("--------------------");
    esta_cali = 0;
    }
    else
    {
                          // lectura promedio
      long reading = 0;
      for (int jj=0;jj<int(avg_size);jj++)
      {
        reading+=hx711.read();
      }
                         // cálculo de masa basado en calibración y ajuste lineal      
      reading/=long(avg_size);
      float ratio_1 = (float) (reading-x0a);
      float ratio_2 = (float) (x1a-x0a);
      float ratio = ratio_1/ratio_2;
      float mass = peso_cono_pesa*ratio;
      peso = int(mass);
      if (peso>=0)
      {
        peso_visu.setValue(peso);
      }
      else
      {
        peso_visu.setValue(0);
      }
    }
                             //Proceso de dosificacion en automático
  if(valo_peda==HIGH)
  {
    EEPROM.get(dire2d, peso_llen_dosi);
    EEPROM.get(dire5d, peso_vuel_dosi);
    peso_dosi = (peso) + ((peso_llen_dosi) - (peso_vuel_dosi));
    peso_dosi_visu = (peso) + (peso_llen_dosi);
    visu_peso_dosi.setValue(peso_dosi_visu);
    dosi_auto = true;
  }
  if(dosi_auto)
  {
    if (inic_dosi==0)
    {
      delay(1000);
      digitalWrite(moto, 0);
      inic_dosi = 1;
    }
    if (inic_dosi==1)
    {
      if (peso >= peso_dosi)       
      {
        digitalWrite(moto, 1);
        inic_dosi = 0;
        dosi_auto = false;
      }
    }
  }
  else
  {
    if (peso <= (5))       
      {
        peso_dosi = (0);
        visu_peso_dosi.setValue(peso_dosi);
      }
    EEPROM.get(dire1c, peso_cono);
    lect_peso_cono.setValue(peso_cono);
    EEPROM.get(dire2c, peso_llen);    
    lect_peso_llen.setValue(peso_llen);
    visu_peso_llen.setValue(peso_llen);
    EEPROM.get(dire5c, peso_vuel);    
    lect_peso_vuel.setValue(peso_vuel);
    EEPROM.get(dire3b, x0a);
    EEPROM.get(dire4b, x1a);
    EEPROM.get(dire1c, peso_cono_pesa);
    delay(300);
  }
                //
    EEPROM.get(dire3b, x0a);
    EEPROM.get(dire4b, x1a);
    EEPROM.get(dire1c, peso_cono_pesa);
    delay(30);
    
}             //Fin
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
