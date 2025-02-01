#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"

// Definição dos GPIOs dos LEDs e Botão
#define LED_GREEN 11
#define LED_YELLOW 12
#define LED_RED 13
#define BUTTON 5
#define DEBOUNCE_TIME 200 // 200ms de debounce

volatile bool button_pressed = false;
volatile uint32_t last_press_time = 0;
volatile int led_state = 3;
volatile bool led_sequence_active = false; // Flag para evitar reinício indesejado
volatile bool pause_semaphore = false; // Flag para pausar o semáforo

// Callback do temporizador periódico (Semáforo automático)
bool repeating_timer_callback(struct repeating_timer *t) {
    static int state = 0;
    if (!pause_semaphore) {
        gpio_put(LED_GREEN, state == 0);
        gpio_put(LED_YELLOW, state == 1);
        gpio_put(LED_RED, state == 2);
        printf("Semáforo: %s\n", state == 0 ? "Verde" : state == 1 ? "Amarelo" : "Vermelho");
        state = (state + 1) % 3;
    }
    return true; // Mantém o timer ativo
}

// Callback para o temporizador one-shot (Desliga LEDs em sequência)
int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    if (led_state > 0) {
        gpio_put(LED_RED, led_state > 2);
        gpio_put(LED_YELLOW, led_state > 1);
        gpio_put(LED_GREEN, led_state > 0);
        printf("Estado do LED: %d LEDs acesos\n", led_state);
        led_state--;

        // Agendar próxima execução em 3s se ainda houver LEDs para desligar
        if (led_state > 0) {
            add_alarm_in_ms(3000, turn_off_callback, NULL, false);
        } else {
            button_pressed = false; // Permitir novo acionamento do botão
            led_sequence_active = false; // Libera para nova ativação
            pause_semaphore = false; // Retoma o semáforo
            printf("Todos os LEDs apagados, semáforo retomado.\n");
        }
    }
    return 0; // Retorno esperado para alarm callback
}

int main() {
    stdio_init_all();
    printf("Inicializando sistema...\n");
    
    // Configuração dos GPIOs
    gpio_init(LED_RED);
    gpio_init(LED_YELLOW);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_YELLOW, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
    
    // Configuração do temporizador periódico (Semáforo automático)
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);
    
    // Loop principal
    while (1) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        if (!gpio_get(BUTTON) && (current_time - last_press_time) > DEBOUNCE_TIME && !button_pressed && !led_sequence_active) {
            last_press_time = current_time;
            button_pressed = true;
            led_sequence_active = true; // Bloqueia novas ativações durante o ciclo
            pause_semaphore = true; // Pausa o semáforo
            printf("Botão pressionado, esperando ciclo do semáforo encerrar.\n");

            // Espera até o próximo ciclo do semáforo para iniciar os LEDs
            sleep_ms(3000);
            
            printf("Acionando LEDs pelo botão.\n");
            led_state = 3;
            gpio_put(LED_RED, true);
            gpio_put(LED_YELLOW, true);
            gpio_put(LED_GREEN, true);
            add_alarm_in_ms(3000, turn_off_callback, NULL, false);
        }
        printf("Sistema em execução...\n"); // Mensagem a cada segundo
        sleep_ms(1000);
    }
}
