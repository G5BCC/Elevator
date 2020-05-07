# Elevator
Elevator system written C++ for Arduino.

## To-do list:
### Events:
- [X] ON: botão para iniciar a operação do elevador é apertado
- [X] OFF: botão para terminar a operação do elevador é apertado
- [X] porta_fecha: botão para fechar a porta do elevador é apertado
- [ ] alinhamento: elevador alinha-se com um andar
- [X] emergência: botão de emergência é apertado
- [ ] destinação: seleção de um andar para ir
- [ ] chamada_S: chamada do elevador em algum andar  para subir
- [ ] chamada_D: chamada do elevador em algum andar para descer
- [ ] demanda: pode ser um evento de destinação, chamada_S ou chamada_D

### Actions:
- [ ] marcar_destino(x): marcar o andar x como destino para o elevador estacionar
- [ ] marcar_S(x): marca o andar x para estacionar na subida
- [ ] marcar_D(x): marca o andar x para estacionar na descida
- [X] marcar_emergencia: marca que o botão de emergência foi acionado
- [ ] desmarcar_destino(x): desmarca o andar x como destino para oelevador estacionar
- [ ] desmarcar_S(x): desmarca o andar x para estacionar na subida
- [ ] desmarcar_D(x): desmarca o andar x para estacionar na descida
- [X] desmarcar_emergencia: desmarca que o botão de emergência foi acionado

### Conditions:
- [X] ha_emergencia: verifica se está marcado que o botão de emergência foi acionado
- [ ] ha_destino(x): verifica se o andar x está marcado como destino para o elevador
- [ ] ha_chamada_S(x): verifica se o andar x está marcado para estacionar nasubida
- [ ] ha_chamada_D(x): verifica se o andar x está marcado para estacionar nadescida
- [ ] ha_demanda_acima(x): verifica se há algum andar acima do andar x que esteja marcado para estacionar (na subida ou na descida) ou como destino para o elevador
- [ ] ha_demanda_abaixo(x): verifica se há algum andar abaixo do andar x que esteja marcado para estacionar (na subida ou na descida ou como destino para o elevador