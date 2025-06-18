# Atividade 2 projeto em c
## Buscador de Valor por Timestamp para Sensor

Este programa em C permite buscar, para um dado sensor, o valor mais próximo de um timestamp fornecido. Ele utiliza os arquivos .dat, e realiza uma busca em arquivos ordenados em ordem decrescente para encontrar a medida com o timestamp mais próximo ao informado.

---

## Como funciona

O programa recebe dois argumentos na linha de comando:

1. O nome do sensor (que corresponde ao arquivo `<sensor>.dat` contendo os dados).
2. O timestamp de referência.

Ele abre o arquivo correspondente, lê todas as medições (timestamp e valor), e utiliza uma busca binária para encontrar o registro cujo timestamp seja o mais próximo do timestamp informado. Ao final, imprime no terminal o timestamp encontrado e o valor associado.

---

## Como compilar

Para compilar o programa, use o comando:

```bash
gcc -o consulta consulta.c

./consulta

Digite o nome do sensor: sensor1

Digite o timestamp desejado: 1700000100

Valor mais próximo do timestamp 1700000100:
1750106403 8.10
