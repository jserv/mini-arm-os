void itoa(int i, char *a)
{
    if(i<=0){
        if(i==0){
            *a = '0';
        }else{
            *a = 'X';
        }
        *(a+1) = '\0';
        return;
    }
    
    while(i > 0){
        int mod = i%10;
        *a++ = mod + '0'; // little endian
        i/=10;
    }
    *a = '\0';
    return;
}
