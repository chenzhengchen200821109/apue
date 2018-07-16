#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define MAXSTRINGSZ 4096

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
extern char **environ;
int mflag = 0; /* malloc() invoke flag */

/*
 * thread safe
 */
int putenv_v(char *string)
{
    int i, lenName, len;
    char *p;
    char **newEnviron;
    int nEntry;

    pthread_mutex_lock(&mutex);
    for (i = 0; environ[i] != NULL; i++)
        ;
    nEntry = i + 1; /* including NULL pointer */
    if ((p = strchr(string, '=')) == NULL) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    lenName = p - string;
    len = strlen(string);
    for (i = 0; environ[i] != NULL; i++) {
        if (strncmp(string, environ[i], lenName) == 0) { /* name found */
            if (strlen(environ[i]) >= len) {
                strncpy(environ[i], string, len + 1);
                pthread_mutex_unlock(&mutex);
                return (0);
            } else {
                if ((p = calloc(len + 1, sizeof(char))) == NULL) {
                    pthread_mutex_unlock(&mutex);
                    return (-1);
                }
                strncpy(p, string, len + 1);
                environ[i] = p;
                pthread_mutex_unlock(&mutex);
                return (0);
            }
        }
    }
    /* name not found */
    if (mflag == 0) {
        if ((p = calloc(len + 1, sizeof(char))) == NULL) {
            pthread_mutex_unlock(&mutex);
            return (-1);
        }
        if ((newEnviron = calloc(nEntry + 1, sizeof(char **))) == NULL) {
            pthread_mutex_unlock(&mutex);
            return (-1);
        }
        for (i = 0; i < nEntry - 1; i++) {
            newEnviron[i] = environ[i];
        }
        strncpy(p, string, len + 1);
        newEnviron[nEntry - 1] = p;
        newEnviron[nEntry] = NULL;
        environ = newEnviron;
        mflag = 1;
        pthread_mutex_unlock(&mutex);
        return (0);
    } else {
        if ((newEnviron = (char **)realloc(environ, nEntry + 1)) == NULL) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
        environ = newEnviron;
        if ((p = calloc(len + 1, sizeof(char))) == NULL) {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
        strncpy(p, string, len + 1);
        environ[nEntry - 1] = p;
        environ[nEntry] = NULL;
        mflag = 1;
        pthread_mutex_unlock(&mutex);
        return (0);
    }
        
}
