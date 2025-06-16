#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256


typedef struct 
{
    long timestamp;
    char value[64];
} Measure;


int compare(const void* a, const void* b) 
{
    long ts_a = ((Measure*)a)->timestamp;
    long ts_b = ((Measure*)b)->timestamp;
    if (ts_a < ts_b) return -1;
    if (ts_a > ts_b) return 1;
    return 0;
}


int main(int argc, char* argv[]) 
{
    if (argc != 3) 
    {
        printf("Uso: %s <nome_sensor> <timestamp>\n", argv[0]);
        return 1;
    }

    char* sensor = argv[1];
    long target_time = atol(argv[2]);

    
    char filename[128];
    snprintf(filename, sizeof(filename), "%s.dat", sensor);

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo '%s'. Verifique se o sensor existe.\n", filename);
        return 1;
    }

    
    Measure* measures = NULL;
    int capacity = 100;
    int count = 0;

    measures = malloc(capacity * sizeof(Measure));
    if (!measures) 
    {
        printf("Erro ao alocar memoria.\n");
        fclose(fp);
        return 1;
    }

    char line[MAX_LINE];
     while (fgets(line, sizeof(line), fp)) 
{
    Measure m;
    char sensor_name[64]; 

    if (sscanf(line, "%s %ld %[^\n]", sensor_name, &m.timestamp, m.value) == 3) 
    {
        if (count >= capacity) 
        {
            capacity *= 2;
            measures = realloc(measures, capacity * sizeof(Measure));
        }
        measures[count++] = m;
    }
}


    fclose(fp);

    if (count == 0) 
    {
        printf("Nenhum dado encontrado para o sensor '%s'.\n", sensor);
        free(measures);
        return 1;
    }

    
    int left = 0;
    int right = count - 1;
    int closest_index = 0;

    while (left <= right) 
    {
        int mid = (left + right) / 2;
        long diff_mid = labs(measures[mid].timestamp - target_time);

        if (mid < count - 1) 
        {
            long diff_next = labs(measures[mid + 1].timestamp - target_time);
            if (diff_next < diff_mid)
             {
                if (mid + 1 < count) 
                {
                    mid++;
                }
            }
        }

        if (mid > 0) 
        {
            long diff_prev = labs(measures[mid - 1].timestamp - target_time);
            if (diff_prev < diff_mid) 
            {
                mid--;
            }
        }

        closest_index = mid;

        if (measures[mid].timestamp < target_time)
            left = mid + 1;
        else if (measures[mid].timestamp > target_time)
            right = mid - 1;
        else
            break; 
    }

    
    for (int i = 0; i < count; i++) 
    {
        if (labs(measures[i].timestamp - target_time) < labs(measures[closest_index].timestamp - target_time)) 
        {
            closest_index = i;
        }
    }

    
    printf("%ld %s\n", measures[closest_index].timestamp, measures[closest_index].value);

    
    free(measures);
    return 0;
}
