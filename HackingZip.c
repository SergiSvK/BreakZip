#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Ataque de furza bruta, paratiendo de la base que la contraseña solo puede tener un max de 3 caracteres
 * @param argv .zip
 * @return la contraseña y el .zip abierto
 */
int main(char *argv[]) {

    char alfabeto[] = {
            'A','B','C','D','E','F','G','H','I','J','K','L',
            'M','N','O','P','Q','R','S','T','U','V','W','X',
            'Y','Z','0','1','2','3','4','5','6','7','8','9'
    };

    //procesos hijos del programa
    int numHijos = 3;
    int lenght = sizeof(alfabeto)/sizeof(char);

    //la longitud del alfabeto por la cantidad de hijo a funcionar
    int cantidad = lenght / numHijos;
    int min = 0; //cantidad mínima
    int max = min + cantidad; // sera la cantidad + el min
    pid_t hijos[numHijos];
    int estado;


    for (int i = 0; i < numHijos; i++)
    {
        int pd[2];
        pipe(pd);

        pid_t hijo = fork();
        hijos[i] = hijo;

        if (hijo == -1)
        {
            //Error de creacción
            exit(-1);
        }else if(hijo == 0)
        {
            char buffer[2];
            //0 de lecutura
            read(fd[0]],buffer, sizeof(buffer));
            sscanf(buffer, "%d",&min);

            read(fd[0]],buffer, sizeof(buffer));
            sscanf(buffer, "%d",&max);

            if (buscarContrasenya(lenght,min,max, argv))
            {
                exit(0);
            }else
            {
                exit(-1);
            }

        }else
        {
            char msg[2];
            sprintf(msg,"%d",min);
            // 1 de escritura
            write(fd[1],msg, sizeof(msg));

            sprintf(msg,"%d",max);
            write(fd[1],msg, sizeof(msg));

            min = max;
            max = min + cantidad;

        }

    }

    while (1)
    {
        int pid = whait(&estado);
        if (pid == -1)
        {
            printf("Error, no se ha encontrado la contraseña");
            exit(-1);
        }else if(pid == 0)
        {
            for (int i = 0; i < numHijos; i++)
            {
                //para matar el proceso
                kill(SIGKILL, pid[i]);
            }
            exit(0);
        }else
        {
            printf("No se ha encontrado el proceso anterior");
        }

    }

}


/***
 * Busca una contraseña entre los parametros max y min del array de alfabeto  del archivo que se le pasa e
 * intenta abrir el archivo en cuestión
 * @param lenght tamaño
 * @param min posición min
 * @param max posición max
 * @param file archivo a abrir
 * @return 0 si no se puede abrir y 1 si se habre
 */
int buscarContrasenya(int lenght, int min, int max, const char alfabeto[], char file[]){


    char contrasenya[4];

    for (int i = min; i < max; i++)
    {
        for (int j = 0; i < lenght; j++)
        {
            for (int k = 0; i < lenght; k++)
            {
                contrasenya[0] = alfabeto[i];
                contrasenya[1] = alfabeto[j];
                contrasenya[2] = alfabeto[k];
                contrasenya[3] = '\0';

                if (abrirZip(contrasenya,file))
                {
                    return 1;
                }
            }
        }
    }
    return 0;

}


/**
 * Se le pasa la clave y al archivo a abrir
 * Anotación: los parametros /dev/null 2&1 lo usaremos para desacer las salida que no necesita el usuario.
 * Los programas y procesos pueden generar registros de mensajes de información de salida de gran longitud.
 * STDERR(Error estándar) en linux y está re presentado por el "2" y el éxito  STDOUT represenatdo por "1"
 * @param clave a probar, empezadando por AAA y acabando por 999
 * @param archivo .zip
 * @return devuelve 0 si no se ha encotrado y 1 si se ha podido abrir
 * Info de la retroalimentacion de linux: https://www.linuxshelltips.com/redirect-output-in-linux/
 */
int abrirZip(char clave[], char archivo[])
{
    char comando[80];
    //monataje del comando de consola a ejecutar, se le pasa a comando todos los parametros
    sprintf(comando, "unzip -op -P %s %s /dev/null 2&1", archivo, clave);

    if (system(comando) == 0)
    {
        //muestra por terminal en el caso que se haya encontrado la clave y muestra
        printf("La contraseña se ha encontrado %s\n",clave);
        return 1;
    }
    //no se puedo encontrar
    return 0;
}