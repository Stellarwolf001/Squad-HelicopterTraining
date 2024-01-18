/*�˴�����Ҫ����Ϊ��
 *ʶ����־�л�ͼ��ʱ�����
 *��ʵ��ʱ�����Աȣ�
 *��ֵС��15s��ִ�м��ټ�ȡ������ָ��
 *���˸ո����ţ��������Ϊǳ�ԣ����������
 *
 *�뽫�������ݽ����޸�
 * 23�� *rconPath = "C:\\Users\\Administrator\\Desktop\\rcon.exe";
 * 24�� *serverPassword = "-PYourRCONPassword";
 * 25�� *serverPort = "-p21114";
 * 26�� *log_address = "C:\\Users\\Administrator\\Desktop\\squad_server\\SquadGame\\Saved\\Logs\\SquadGame.log";
 * 
 * Squad�쾯_Stellarwolf��д
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ע���滻�����·��Ϊʵ�ʵ� rcon.exe �ļ�·��
const char *rconPath = "C:\\Users\\Administrator\\Desktop\\rcon.exe";
const char *serverPassword = "-PYourRCONPassword";
const char *serverPort = "-p21114";
const char *log_address = "C:\\Users\\Administrator\\Desktop\\squad_server\\SquadGame\\Saved\\Logs\\SquadGame.log";

// ���������ַ�������
const char *commands[] = 
{
    "AdminSlomo 20",
    "AdminForceAllVehicleAvailability 1",
    "AdminForceAllRoleAvailability 1",
    "AdminForceAllDeployableAvailability 1",
    "AdminForceAllRoleAvailability 1",
    "AdminDisableVehicleTeamRequirement 1",
    "AdminDisableVehicleKitRequirement 1",
    "AdminDisableVehicleClaiming 1",
    "AdminNoRespawnTimer 1",
    "AdminSlomo 1"
};

//��ȡ��ǰʱ���ʱ���
time_t currentTime;

//�����������ַ���
char fullCommand[512];

// ����ṹ��洢���ں�ʱ��
struct DateTime 
{
    char year[5];
    char month[3];
    char day[3];
    char hour[3];
    char minute[3];
    char second[3];
    char millisecond[4];
}logDateTime;                

//����־�е��¡��ա�ʱ���֡�����ϳ�һ������
unsigned int LogcombineDateTime_sec(char line[],struct DateTime logDateTime) 
{
    int result = 0,mouth,day,hour,minute,second;// ��ȡ�ꡢ�¡��ա�Сʱ�����ӡ���
    sscanf(line, "[%4[^.].%2[^.].%2[^-]-%2[^.].%2[^.].%2[^:]:%2[^.].%2[^.]:%3[^]]", logDateTime.year, logDateTime.month, logDateTime.day, logDateTime.hour, logDateTime.minute, logDateTime.second, logDateTime.millisecond);
    mouth = strtol(logDateTime.month, NULL, 10) * 2592000;
    day = strtol(logDateTime.day, NULL, 10) * 86400;
    hour = strtol(logDateTime.hour, NULL, 10) * 3600;
    minute = strtol(logDateTime.minute, NULL, 10) * 60;
    second = strtol(logDateTime.second, NULL, 10);
    result = mouth + day + hour + minute + second + 28800;

    return result;
}

//����ǰʱ����¡��ա�ʱ���֡�����ϳ�һ������
unsigned int LocalcombineDateTime_sec(struct tm *localTime) 
{
    unsigned int result;

    result = (localTime->tm_mon + 1) * 2592000+
    (localTime->tm_mday) * 86400+
    (localTime->tm_hour) * 3600+
    (localTime->tm_min) * 60+
    (localTime->tm_sec);

    return result;
}

//�ж�ʱ��
int IsTimeDifferenceWithinThreshold(unsigned int LogcombinedDateTime)
{
    //���µ�ǰʱ��
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    
    unsigned int LocalcombinedDateTime = LocalcombineDateTime_sec(localTime);

    if( (LocalcombinedDateTime - LogcombinedDateTime) < 15 )
        return 1;
    else
        return 0;
}

int executeCommand(const char *command, char *output, size_t outputSize) 
{
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
        return -1;
    }

    // ��ȡ�������
    size_t bytesRead = fread(output, 1, outputSize - 1, fp);
    if (bytesRead == 0) {
        perror("Error reading from pipe");
        pclose(fp);
        return -1;
    }

    output[bytesRead] = '\0';  // ����ַ���������

    // �ر��ļ�ָ��
    int status = pclose(fp);
    if (status == -1) {
        perror("Error closing pipe");
        return -1;
    }

    return 0;
}

//ִ��ָ��
int Skip_Time(struct tm *localTime)
{
    //��ӡִ��ָ���ʱ��
    printf("Skip_Time\n");
    printf("localtime = [%d.%d.%d-%d:%d:%d]\n\n",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);

    //�ȴ�5��
    system("timeout /t 5 /nobreak >nul");

    //��ȡ��������ĳ���
    size_t numCommands = sizeof(commands) / sizeof(commands[0]);

    for (size_t i = 0; i < numCommands; i++) 
    {
        snprintf(fullCommand, sizeof(fullCommand), "%s %s %s %s", rconPath, serverPort, serverPassword, commands[i]);

        //ִ��ָ��
        int result = system(fullCommand);

        //���ִ�н��
        if (result != 0) 
        {
            fprintf(stderr, "Failed to execute command: %s\n", fullCommand);
            return 1;
        }

        //�ȴ�1��
        system("timeout /t 1 /nobreak >nul");
    }
}

int main() 
{    
    FILE *file;
    char line[512];

    //��ȡ��ǰʱ���ʱ���
    time(&currentTime);

    //ʹ�� localtime ������ʱ���ת��Ϊ����ʱ��
    struct tm *localTime = localtime(&currentTime);

    printf("�ű������ɹ�����ǰʱ��Ϊ��\n");
    printf("localtime = [%d.%d.%d-%d:%d:%d]\n\n",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);

    while(1)
    {
        file = fopen(log_address, "r");

        if (file == NULL) 
        {
            perror("Error opening file");
            fclose(file);
            return 1;
        }

        while (fgets(line, sizeof(line), file)) 
        {
            // ����Ƿ�����ؼ���
            if(strstr(line, "StartLoadingDestination") != NULL) 
            {
                if(IsTimeDifferenceWithinThreshold(LogcombineDateTime_sec(line,logDateTime)))
                {   
                    Skip_Time(localTime);
                }
                else
                {
                    //�ȴ�1��
                    system("timeout /t 1 /nobreak >nul");
                }
            }
            if(strstr(line, "Join succeeded") != NULL)
            {
                if(IsTimeDifferenceWithinThreshold(LogcombineDateTime_sec(line,logDateTime)))
                {
                    //�����������ShowServerInfoָ���ȡ������״̬
                    snprintf(fullCommand, sizeof(fullCommand), "%s %s %s %s", rconPath, serverPort, serverPassword, "ShowServerInfo");

                    char commandOutput[1024]; // �ʵ���С�Ļ����������ڱ����������

                    //ִ��ָ���ȡ���
                    executeCommand(fullCommand, commandOutput, sizeof(commandOutput));

                    //����"PlayerCount_I"�ֶ�
                    const char *playerCountStart = strstr(commandOutput, "\"PlayerCount_I\":\"");
                    if (playerCountStart != NULL) 
                    {
                        //��λ�����ֵ���ʼλ��
                        const char *countStart = playerCountStart + strlen("\"PlayerCount_I\":\"");
                        
                        //ʹ��sscanf��ȡ���ּ�������������
                        int playerCount;
                        if (sscanf(countStart, "%d", &playerCount) == 1) 
                        {
                            //�������ڽ���һ�ˣ�ִ��ָ��
                            if(playerCount == 1)
                            {
                                Skip_Time(localTime);
                            }
                        } 
                        else 
                        {
                            printf("Failed to extract player count.\n");
                        }
                    } 
                    else 
                    {
                        printf("PlayerCount_I not found in the string.\n");
                    }
                }
                else
                {
                    //�ȴ�1��
                    system("timeout /t 1 /nobreak >nul");
                }
            }
        }
        fclose(file);
    }

    return 0;
}