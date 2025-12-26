#pragma GCC optimize("Ofast")

#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

/* Incluindo exemplos do LVGL */

//#include <examples/lv_examples.h>
//#include <../../examples/lv_examples.h>
//#include <../examples/lv_examples.h>
//#include <../../lv_examples.h>
//#include <../lv_examples.h>

//#include <examples/lv_examples.h>
#include <../examples/lv_examples.h>
//#include <examples/widgets/lv_example_widgets.h>
#include <../examples/widgets/lv_example_widgets.h>
//#include <demos/lv_demos.h>
#include <../demos/lv_demos.h>

/* ======================================================
   TOUCH – CYD (XPT2046)
   ====================================================== */
#define XPT2046_CS    33
#define XPT2046_IRQ   36
#define XPT2046_CLK   25
#define XPT2046_MISO  39
#define XPT2046_MOSI  32

SPIClass touchSPI(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

/* ======================================================
   DISPLAY
   ====================================================== */
TFT_eSPI tft;

/* ======================================================
   RESOLUÇÃO (CYD ROTATION 1)
   ====================================================== */
static const uint32_t screenWidth  = 320;
static const uint32_t screenHeight = 240;

/* ======================================================
   LVGL DRAW BUFFER
   ====================================================== */
static lv_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 20];

/* ======================================================
   LVGL TICK
   ====================================================== */
static uint32_t last_tick = 0;

/* ======================================================
   DISPLAY FLUSH
   ====================================================== */
void my_disp_flush(lv_display_t *disp,
                   const lv_area_t *area,
                   uint8_t *px_map)
{
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)px_map, w * h, true);
  tft.endWrite();

  lv_display_flush_ready(disp);
}

/* ======================================================
   TOUCH READ – CORRIGIDO
   ====================================================== */
void my_touch_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();

    data->state = LV_INDEV_STATE_PRESSED;

    // Mapeamento correto para CYD + ROTATION 1
    data->point.x = map(p.x, 200, 3800, 0, screenWidth);     // X direto
    data->point.y = map(p.y, 200, 3800, 0, screenHeight);    // Y direto

    // Caso o toque esteja invertido, use:
    // data->point.x = screenWidth - map(p.x, 200, 3800, 0, screenWidth);
    // data->point.y = screenHeight - map(p.y, 200, 3800, 0, screenHeight);
  }
  else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

/* ======================================================
   SETUP
   ====================================================== */
void setup()
{
  Serial.begin(115200);

  /* ---------------- TFT ---------------- */
  tft.begin();
  tft.setRotation(1);  // ROTATION 1 (paisagem)
  tft.fillScreen(TFT_BLACK);

  /* ---------------- TOUCH ---------------- */
  touchSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchSPI.setFrequency(2500000); // ESSENCIAL
  touchscreen.begin(touchSPI);
  touchscreen.setRotation(1);

  /* ---------------- LVGL ---------------- */
  lv_init();

  lv_draw_buf_init(
    &draw_buf,
    screenWidth,
    20,
    LV_COLOR_FORMAT_RGB565,
    screenWidth * sizeof(lv_color_t),
    buf,
    sizeof(buf)
  );

  lv_display_t *disp = lv_display_create(screenWidth, screenHeight);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_draw_buffers(disp, &draw_buf, NULL);

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, my_touch_read);

/* ---------------- LVGL EXEMPLO ---------------- */
  // You can use it by removing the comment symbol (//).
  // demos ------------------------------------------------------------
lv_demo_widgets();          // 34% memory used - Demo Start
//lv_demo_benchmark();        // 38% memory used - Engrenagens
//lv_demo_keypad_encoder();   // 24% memory used - Lista de Números
//lv_demo_music();            // 56% memory used - Música Guitarra
  //get_started -------------------------------------------------------
//lv_example_get_started_3(); // 2 button - 
//lv_example_get_started_1(); // Hello World
//lv_example_get_started_2(); // button couter
//lv_example_get_started_4(); // Slider
  //others ------------------------------------------------------------
//lv_example_observer_1();    // Slider
//lv_example_observer_2();    // logged out - Teclado
//lv_example_observer_3();    // time & set - Relógio
//lv_example_observer_4();    // Slider & Button
//lv_example_observer_5();    // update
//lv_example_observer_6();    // 10 button
  //widgets -----------------------------------------------------------
//lv_example_image_3(); // engrenagem
//lv_example_animimg_1(); //copo de achocolatado
//lv_example_arc_1();
//lv_example_arc_2();
//lv_example_bar_1();
//lv_example_bar_2();
//lv_example_bar_3();
//lv_example_bar_4();
//lv_example_bar_5();
//lv_example_bar_6();
//lv_example_bar_7();
//lv_example_button_1();
//lv_example_button_2();      
//lv_example_button_3();
//lv_example_buttonmatrix_1();
//lv_example_buttonmatrix_2();
//lv_example_buttonmatrix_3();
//lv_example_calendar_1();
//lv_example_canvas_1 fail
//lv_example_canvas_2();
//lv_example_canvas_3();
//lv_example_canvas_4();
//lv_example_canvas_5();
//lv_example_canvas_6();
//lv_example_canvas_7();
//lv_example_canvas_8();
//lv_example_chart_1();
//lv_example_chart_2();
//lv_example_chart_3();
//lv_example_chart_4();
//lv_example_chart_5();
//lv_example_chart_6();
//lv_example_chart_7();
//lv_example_chart_8 fail
//lv_example_checkbox_1();
//lv_example_checkbox_2();
//lv_example_dropdown_1();
//lv_example_dropdown_2();
//lv_example_dropdown_3();
//lv_example_image_1();
//lv_example_image_2();
//lv_example_image_4();
//lv_example_imagebutton_1_fail
//lv_example_keyboard_1();
//lv_example_keyboard_2();
//lv_example_label_1();
//lv_example_label_2();
//lv_example_label_3 fail
//lv_example_label_4 fail
//lv_example_label_5();
//lv_example_led_1
//lv_example_line_1
//lv_example_menu_1
//lv_example_menu_2
//lv_example_menu_3
//lv_example_menu_4
//lv_example_menu_5
//lv_example_msgbox_1
//lv_example_obj_1
//lv_example_obj_2
//lv_example_roller_1
//lv_example_roller_2 fail
//lv_example_roller_3 fail
//lv_example_scale_1();
//lv_example_scale_2();
//lv_example_scale_3();
//lv_example_scale_4();
//lv_example_scale_5();
//lv_example_slider_1();
//lv_example_slider_2();
//lv_example_slider_3();
//lv_example_slider_4();
//lv_example_span_1();
//lv_example_spinbox_1();
//lv_example_spinner_1();
//lv_example_switch_1();
//lv_example_table_1();
//lv_example_table_2();
//lv_example_tabview_1();
//lv_example_tabview_2();
//lv_example_textarea_1();
//lv_example_textarea_2();
//lv_example_textarea_3();
//lv_example_tileview_1();
//lv_example_tileview_2 fail
//lv_example_win_1();
  //-------------------------------------------------------------------


  last_tick = millis();
}

/* ======================================================
   LOOP
   ====================================================== */
void loop()
{
  lv_timer_handler();

  uint32_t now = millis();
  lv_tick_inc(now - last_tick);
  last_tick = now;

  delay(5);
}