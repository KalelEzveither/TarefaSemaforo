# README - Temporizador de um Disparo (One Shot)

## Descrição
Este projeto implementa um temporizador "One Shot" utilizando a Raspberry Pi Pico W. O sistema aciona e desliga LEDs em sequência após o pressionamento de um botão, utilizando a API `add_alarm_in_ms()` do Pico SDK.

## Componentes Utilizados
- **Microcontrolador:** Raspberry Pi Pico W
- **LEDs:** 3 LEDs (azul, vermelho e verde)
- **Resistores:** 3 resistores de 330Ω
- **Botão:** Pushbutton

## Funcionamento
1. Ao pressionar o botão (conectado ao GPIO 5), os três LEDs acendem simultaneamente.
2. Após 3 segundos, o LED verde (GPIO 11) desliga.
3. Após mais 3 segundos (totalizando 6 segundos), o LED vermelho (GPIO 13) desliga.
4. Após mais 3 segundos (totalizando 9 segundos), o LED azul (GPIO 12) desliga.
5. Somente quando todos os LEDs estiverem apagados, um novo pressionamento do botão reiniciará o processo.
6. O botão possui debounce por software para evitar leituras incorretas.


## Autor
- **Kalel Ezveither**
