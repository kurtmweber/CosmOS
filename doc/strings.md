
# Cosmos Strings

Looking at the header file [`string.h`](https://github.com/teverett/CosmOS/blob/fat/kernel/sys/string/string.h) you will likely notice that strcpy, ands strcat don't exist, but instead there are strncpy and strncat. We've chosen to use API's that know the size of the destination buffer, in order to reduce the likelihood of accidentally memory overruns.
