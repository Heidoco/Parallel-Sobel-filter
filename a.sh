#!/bin/bash

# Caminho da imagem
IMAGE="img.jpg"

# Lista de argumentos
ARGS=(1 2 4 8 12)

# Arquivo de log
LOGFILE="execution_times.log"

> "$LOGFILE"

# Para cada n de thread
for arg in "${ARGS[@]}"; do
    echo "Executando ./edge com argumento $arg 10 vezes"
    
    # Variáveis para acumular os tempos
    total_real=0
    total_user=0
    total_sys=0

    # 10 vezes
    for ((i=1; i<=50; i++)); do
        echo "Execução $i para Arg $arg"
        
        # Mede o tempo de execução e captura os tempos
        output=$( { time ./edge_opemp "$arg" "$IMAGE"; } 2>&1 )
        
        real_time=$(echo "$output" | grep "real" | awk '{print $2}')
        user_time=$(echo "$output" | grep "user" | awk '{print $2}')
        sys_time=$(echo "$output" | grep "sys" | awk '{print $2}')

        total_real=$(echo "$total_real + $(echo $real_time | awk -F'm' '{print $1*60 + $2}' | sed 's/s//')" | bc)
        total_user=$(echo "$total_user + $(echo $user_time | awk -F'm' '{print $1*60 + $2}' | sed 's/s//')" | bc)
        total_sys=$(echo "$total_sys + $(echo $sys_time | awk -F'm' '{print $1*60 + $2}' | sed 's/s//')" | bc)
    done

    # média dos tempos
    avg_real=$(echo "$total_real / 50" | bc -l)
    avg_user=$(echo "$total_user / 50" | bc -l)
    avg_sys=$(echo "$total_sys / 50" | bc -l)

    echo "Arg $arg - Média dos Tempos:" >> "$LOGFILE"
    echo "Média real: ${avg_real}s" >> "$LOGFILE"
    echo "Média user: ${avg_user}s" >> "$LOGFILE"
    echo "Média sys: ${avg_sys}s" >> "$LOGFILE"
    echo "-----------------------------" >> "$LOGFILE"
done
