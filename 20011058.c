#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void keyToLock(char *key, char* lock, int left, int right);
int partition(char *arr, int left, int right, int pivotValue);
void swap(char *a, char *b);
void printArray(char *arr, int n);

int main(){
	
	srand(time(NULL)); 												//for generate random numbers
	
	char key[] = {'A', 'B', 'E', 'L','D', 'C', 'F'};
	char lock[] = {'B', 'L' ,'D', 'F', 'E', 'C', 'A'};   
	/*
	char key[] = {'A', 'B', 'C', 'D','E', 'F'};
	char lock[] = {'A', 'B', 'C', 'D','E', 'F'};*/
	/*
	char key[] = {'A', 'B', 'C', 'D','E', 'F'};
	//char lock[] = {'A', 'B', 'C', 'D','E', 'F', 'G'};
	char lock[] = {'F' ,'E', 'D', 'C', 'B', 'A'}; */
	
	int N = sizeof(key) / sizeof(char);
	printf("Keys:\n");
    printArray(key, N);
	printf("Locks:\n"); 
    printArray(lock, N);
	printf("Matched keys and locks are : \n"); 
    keyToLock(key, lock, 0, N-1);
    
    printf("Keys:\n");
    printArray(key, N);
	printf("Locks:\n"); 
    printArray(lock, N); 
    return 0;
}

/*
@brief 

@param key array that contains the values of keys
@param lock array that contains the values of locks
@param left first index of array
@param right last index of array
return
*/

void keyToLock(char *key, char* lock, int left, int right){
	if(left >= right) return;													//sol index sag indexe esit ya da buyuk olduysa demek ki her elemana gore duzenlenmis
	int randomIndex = left + ( rand() % (right-left) ) + 1;						//uygun kosullarda rastgele anahtar seciyoruz.
	int pivotIndex = partition(lock, left, right,  key[randomIndex]);			//partition fonksiyonu ile diziyi duzenliyoruz uygun olarak ve bize pivotun indexini doner.
	partition(key, left, right, lock[pivotIndex]);								//simdi sira kilitleri siralamakta. pivotIndex ile diziyi 
																				// hangi numarali kilide gore duzenleyecegimizi biliyoruz
	keyToLock(key, lock, left, pivotIndex-1);									//artik pivot degerlerine gore anahtar ve kilit dizileri duzenlendi. 
	keyToLock(key, lock, pivotIndex+1, right);									//artýk geriye kalan duzenlenmesi gereken dizi parcalarinda sira
																				//sag ve sol diziler de kendi aralarinda duzenlenecek
}

/*
@brief 

@param arr array that contains the values
@param left first index of array
@param right last index of array
@param pivotValue value of key
return j index of the pivot in the array
*/

int partition(char *arr, int left, int right, int pivotValue){
	int i,j;
	i = left;									//first index of array
	j = right;									//last index of array
	while(i < j){								//sagdan i ve soldan j gelecek. kucuk sayilarin sola buyuklerin de sola gecmesi icin belli baslý kontroller ve yaklasimlar var.
		while(arr[i] <= pivotValue){			//eger pivottan kücükse demek ki yerinde zaten asil bana oraya ait olmayan lazim. buyuk olan varsa pivotun olmasi gereken yerin sagina dogru gitmesi gerek.
			if(arr[i] == pivotValue)			//esit oldugu zaman pivotla karsilastim iyi bir sey ama onu guvenli ve daha sonradan bulabilecegim bir yere koymam lazim
				swap(&arr[i], &arr[left]);		//en basta hareketim soldan geldigi için dizinin en soluna koysam benim icin ileride sikinti cikarmayacak.
			i++;
		}
			
		while(arr[j] > pivotValue){				//eger pivottan buyukse oldugu yerde. pivotun solunda olmasý gerek zaten. kucuk olan sayý varsa asýl onun yeri pivotun solu
			j--;
		}
		swap(&arr[j], &arr[i]);					//burada da pivottan buyuk ve kucuk ama olmamasý gereken degerler yer degistiriyor.
	}
	swap(&arr[j], &arr[i]);						//burada bazen olmasý gereken sayilar fazladan yer degistiriyor while icerisinde. ondan gerekli
	swap(&arr[j], &arr[left]);					//artýk solda pivota gore kucuk sagda pivota gore buyuk sayilar yerlesti ama tek eksik pivot dogru yerinde degil.
												//onu da dogru yere getiriyoruz.
	return j; 									//pivotun dizinin icindeki indexini donduruyoruz.
}

/*
@brief a function that excahange the values in the arrray

@param *a pointer of the array
@param *b pointer of the array

@return
*/

void swap(char *a, char *b){
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

/*
@brief a function that prints the given array

@param Arr the array that to be printed
@param n array length

@return
*/
void printArray(char *arr, int n) 
{
	int i; 
    for( i = 0; i < n; i++) 
    { 
        printf("%c ", arr[i]); 
    } 
    printf("\n"); 
} 
