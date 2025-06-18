#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_INPUT 128

typedef struct 
{
    long timestamp;
    char value[64];
} Measure;


int compare(const void* a, const void* b) 
{
    long ts_a = ((Measure*)a)->timestamp;
    long ts_b = ((Measure*)b)->timestamp;
    if (ts_a > ts_b) return -1;
    if (ts_a < ts_b) return 1;
    return 0;
}


#define ABS(x) ((x) < 0 ? -(x) : (x))


int get_valid_input(const char* prompt, char* buffer, size_t size) 
{
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin)) 
        {
            
            buffer[strcspn(buffer, "\n")] = '\0';

            if (buffer[0] != '\0') 
            {
                return 1; 
            } else 
            {
                printf("Entrada vazia. Por favor, digite algo.\n");
            }
        } else 
        {
            printf("Erro ao ler entrada. Tente novamente.\n");
            return 0;
        }
    }
}

int is_numeric(const char* str) 
{
    char* endptr;
    strtol(str, &endptr, 10);
    return (*endptr == '\0');
}

int main() 
{
    char sensor[MAX_INPUT];
    char timestamp_str[MAX_INPUT];
    long target_time;

    
    while (1) 
    {
        if (!get_valid_input("Digite o nome do sensor: ", sensor, sizeof(sensor)))
            continue;

        if (!get_valid_input("Digite o timestamp desejado: ", timestamp_str, sizeof(timestamp_str)))
            continue;

        char* endptr;
        target_time = strtol(timestamp_str, &endptr, 10);
        if (*endptr != '\0') 
        {
            printf("Timestamp inválido. Digite apenas números inteiros.\n");
            continue;
        }

        break; 
    }

    
    char filename[128];
    snprintf(filename, sizeof(filename), "%s.dat", sensor);

    
    FILE* fp = fopen(filename, "r");
    if (!fp) 
    {
        printf("Erro ao abrir o arquivo '%s'. Verifique se o sensor existe.\n", filename);
        return 1;
    }

    
    int capacity = 100;
    int count = 0;
    Measure* measures = malloc(capacity * sizeof(Measure));
    if (!measures) 
    {
        printf("Erro ao alocar memória.\n");
        fclose(fp);
        return 1;
    }

    
    char line[MAX_LINE];
    int line_number = 0;

    while (fgets(line, sizeof(line), fp)) 
    {
        line_number++;

        char sensor_name[64];
        long timestamp;
        char value[64];

       
        line[strcspn(line, "\n")] = '\0';

        
        if (sscanf(line, "%s %ld %[^\n]", sensor_name, &timestamp, value) == 3) 
        {
            if (count >= capacity) 
            {
                capacity *= 2;
                Measure* temp = realloc(measures, capacity * sizeof(Measure));
                if (!temp) 
                {
                    printf("Erro ao realocar memória na linha %d\n", line_number);
                    free(measures);
                    fclose(fp);
                    return 1;
                }
                measures = temp;
            }
            measures[count].timestamp = timestamp;
            strncpy(measures[count].value, value, sizeof(measures[count].value) - 1);
            measures[count].value[sizeof(measures[count].value) - 1] = '\0';
            count++;
        } else 
        {
            printf("Linha %d: Formato inválido ou campos ausentes\n", line_number);
        }
    }

    fclose(fp);

    if (count == 0) 
    {
        printf("Nenhum dado válido encontrado para o sensor '%s'.\n", sensor);
        free(measures);
        return 1;
    }

    
    qsort(measures, count, sizeof(Measure), compare);

    
    int left = 0;
    int right = count - 1;
    int closest_index = 0;

    while (left <= right) 
    {
        int mid = (left + right) / 2;

        if (mid < count - 1 && ABS(measures[mid + 1].timestamp - target_time) < ABS(measures[mid].timestamp - target_time)) 
        {
            mid++;
        }
        if (mid > 0 && ABS(measures[mid - 1].timestamp - target_time) < ABS(measures[mid].timestamp - target_time)) 
        {
            mid--;
        }

        closest_index = mid;

        if (measures[mid].timestamp < target_time) 
        {
            right = mid - 1; 
        } else if (measures[mid].timestamp > target_time) 
        {
            left = mid + 1; 
        } else 
        {
            break;
        }
    }

    
    for (int i = 0; i < count; i++) 
    {
        if (ABS(measures[i].timestamp - target_time) < ABS(measures[closest_index].timestamp - target_time)) 
        {
            closest_index = i;
        }
    }

    
    printf("\nValor mais próximo do timestamp %ld:\n", target_time);
    printf("%ld %s\n", measures[closest_index].timestamp, measures[closest_index].value);

    
    free(measures);

    return 0;
}
