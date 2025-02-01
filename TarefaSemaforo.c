#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"

#define LED_BLUE 12
#define LED_RED 13
#define LED_GREEN 11
#define BUTTON 5

volatile bool is_running = false; // Evita múltiplos disparos durante a execução

// Callback para desligar o LED verde e manter vermelho e azul ligados
int64_t turn_off_green(alarm_id_t id, void *user_data) {
    gpio_put(LED_GREEN, 0);
    return 0;
}

// Callback para desligar o LED vermelho, mantendo apenas o azul ligado
int64_t turn_off_red(alarm_id_t id, void *user_data) {
    gpio_put(LED_RED, 0);
    return 0;
}

// Callback para desligar o LED azul e permitir nova ativação pelo botão
int64_t turn_off_blue(alarm_id_t id, void *user_data) {
    gpio_put(LED_BLUE, 0);
    is_running = false; // Permite um novo ciclo
    return 0;
}

// Callback para iniciar a sequência de desligamento
int64_t start_led_sequence(alarm_id_t id, void *user_data) {
    add_alarm_in_ms(3000, turn_off_green, NULL, false);
    add_alarm_in_ms(6000, turn_off_blue, NULL, false);
    add_alarm_in_ms(9000, turn_off_red, NULL, false);
    return 0;
}

// Função para verificar o estado do botão dentro do loop principal
void check_button() {
    if (!is_running && !gpio_get(BUTTON)) { // Se o botão for pressionado
        sleep_ms(50); // Debounce
        if (!gpio_get(BUTTON)) { // Confirmação da pressão do botão
            is_running = true;
            gpio_put(LED_BLUE, 1);
            gpio_put(LED_RED, 1);
            gpio_put(LED_GREEN, 1);
            add_alarm_in_ms(0, start_led_sequence, NULL, false);
        }
    }
}

int main() {
    stdio_init_all();
    
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
    
    while (1) {
        check_button();
        sleep_ms(100);
    }
}