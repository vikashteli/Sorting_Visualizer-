//Using SDL and standard IO
#include <SDL.h>
#include<iostream>
#include<ctime>
#include<vector>
#include<iomanip>
using namespace std;

//Screen dimension constants
const int WIDTH = 1000;
const int HEIGHT = 600;
const int SIZE = 500;
SDL_Window* window = NULL;
SDL_Renderer* renderer;
SDL_Event event;
bool quit = false;

void GenerateArray(int* arr);
void Swap(int* arr, int i, int j);
void SelectionSort(int* arr);
void PrintArray(int* arr);
void draw(int* arr, int i, int j);

void setup()
{
	//Create window
	window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	if (window == NULL)
	{
		std::cout << "Window could not be created! SDL_Error" << std::endl << SDL_GetError();
	}
	else
	{
		//Get window surface
		renderer = SDL_CreateRenderer(window, -1, 0);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_RenderClear(renderer);

		//Update the surface
		SDL_UpdateWindowSurface(window);
	}
}
bool processEvent()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// if window closed then quit the game
		case SDL_WINDOWEVENT_CLOSE:
		{
			if (window)
			{
				SDL_DestroyWindow(window);
				window = NULL;
				quit = true;
			}
		}
		break;
		// if escape keyboard passed then quit the game
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
		}
		break;
		// quit the game
		case SDL_QUIT:
			quit = 1;
			break;
		}
	}
	return quit;
}


void GenerateArray(int* arr)
{
	srand(time(NULL));
	for (int i = 0; i < SIZE; i++)
	{
		arr[i] = 1 + rand() % HEIGHT;
	}
}


// swapping i and j elements in array
void Swap(int* arr, int i, int j)
{
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}
void SelectionSort(int* arr)
{
	int i, j, index;
	for (i = 0; i < SIZE - 1; i++)
	{
		index = i;
		for (j = i + 1; j < SIZE; j++)
		{
			if (arr[j] < arr[index]) {
				index = j;
			}
		}
		draw(arr, i, index);
		Swap(arr, i, index);
		SDL_Delay(5);
	}
	SDL_Rect rect;
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	rect = { (SIZE - 1) * (WIDTH / SIZE),HEIGHT - arr[SIZE - 1],WIDTH / SIZE,HEIGHT };
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);
}

//bubble sort
void BubbleSort(int* arr)
{
	for (int i = 0; i < SIZE - 1; i++)
	{
		bool swapped = false;

		for (int j = 0; j < SIZE - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				draw(arr, j, j + 1);
				Swap(arr, j, j + 1);
				SDL_Delay(5);
				swapped = true;
			}
		}

		// If no two elements were swapped in inner loop, the array is already sorted
		if (!swapped)
			break;
	}

	// Render the final state of the sorted array
	SDL_Rect rect;
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	rect = { 0, HEIGHT - arr[0], WIDTH / SIZE, HEIGHT };
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderPresent(renderer);
}


//MERGE SORT

void merge2SortedArrays(int a[], int si, int mid, int ei)
{
	int size_output = (ei - si) + 1;
	int* output = new int[size_output];

	int i = si, j = mid + 1, k = 0;

	// Call draw before starting the merge
	draw(a, si, ei);

	while (i <= mid && j <= ei)
	{
		if (a[i] <= a[j])
		{
			output[k] = a[i];
			i++;
		}
		else
		{
			output[k] = a[j];
			j++;
		}
		k++;
	}

	// Copy the remaining elements from the left subarray
	while (i <= mid)
	{
		output[k] = a[i];
		i++;
		k++;
	}

	// Copy the remaining elements from the right subarray
	while (j <= ei)
	{
		output[k] = a[j];
		j++;
		k++;
	}

	// Copy the merged elements back to the original array
	for (int l = 0; l < size_output; l++)
	{
		a[si + l] = output[l];

		// Call draw after each element is merged
		draw(a, si + l, si + l);
		SDL_Delay(5);
	}

	// Call draw after the merge is complete
	draw(a, si, ei);

	delete[] output;
}

void mergeSort(int a[], int si, int ei)
{
	if (si >= ei)
	{
		return;
	}

	int mid = (si + ei) / 2;

	mergeSort(a, si, mid);
	mergeSort(a, mid + 1, ei);

	merge2SortedArrays(a, si, mid, ei);
}



//INSERTION SORT

void InsertionSort(int* arr)
{
	for (int i = 1; i < SIZE; i++)
	{
		int key = arr[i];
		int j = i - 1;

		// Call draw to visualize the array before the insertion
		draw(arr, i, j);

		while (j >= 0 && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			j--;

			// Call draw to visualize the array during the swap
			draw(arr, j + 1, j);
			SDL_Delay(5); // Add a delay for visualization
		}

		arr[j + 1] = key;

		// Call draw to visualize the array after the insertion
		draw(arr, j + 1, i);
		SDL_Delay(5); // Add a delay for visualization
	}

	// Call draw to visualize the fully sorted array
	draw(arr, -1, -1);
	SDL_Delay(5); // Add a delay for visualization
}


//HEAP SORT

void maxHeapify(int* arr, int n, int i)
{
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	// Find the largest element among the root, left child, and right child
	if (left < n && arr[left] > arr[largest])
	{
		largest = left;
	}

	if (right < n && arr[right] > arr[largest])
	{
		largest = right;
	}

	// If the largest element is not the root, swap it with the root
	if (largest != i)
	{
		Swap(arr, i, largest);

		// Call draw to visualize the array after the swap
		draw(arr, i, largest);

		// Recursively maxHeapify the affected subtree
		maxHeapify(arr, n, largest);
	}
}


void heapSort(int* arr)
{
	int n = SIZE;

	// Build a max heap from the input array
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		maxHeapify(arr, n, i);
	}

	// Extract elements from the heap one by one
	for (int i = n - 1; i > 0; i--)
	{
		// Move the current root to the end
		Swap(arr, 0, i);

		// Call draw to visualize the array after the swap
		draw(arr, 0, i);

		// Reduce heap size and max heapify the root element
		maxHeapify(arr, i, 0);
	}

	// Call draw to visualize the fully sorted array
	draw(arr, -1, -1);
}





void PrintArray(int* arr)
{
	for (int i = 0; i < SIZE; i++)
	{
		cout << arr[i] << ' ';
	}
}


void draw(int* arr, int i, int j)
{
	SDL_Rect rect;

	// Clear the screen
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	// Draw each element of the array with different colors
	for (int k = 0; k < SIZE; k++)
	{
		int red = 0;
		int green = 0;
		int blue = 255; // Default color is blue

		if (k == i || k == j)
		{
			// Highlight the elements being compared in a different color
			red = 255;
			blue = 0;
		}

		SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		rect = { k * WIDTH / SIZE, HEIGHT - arr[k], WIDTH / SIZE, arr[k] };
		SDL_RenderFillRect(renderer, &rect);
	}

	// Present the changes
	SDL_RenderPresent(renderer);

	// Add a delay if needed for visualization
	// SDL_Delay(50);
}


int main(int argc, char* args[])
{
	time_t start, end;
	int arr[SIZE];
	GenerateArray(arr);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error" << std::endl << SDL_GetError();
	}
	else
	{
		setup();

		SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
		for (int i = 0; i < SIZE; i++)
		{
			SDL_Rect rect = { i * WIDTH / SIZE,HEIGHT - arr[i],WIDTH / SIZE,HEIGHT };
			SDL_RenderFillRect(renderer, &rect);
			SDL_RenderPresent(renderer);
		}

		int num;
		cout << "Enter the code for the algorithm to execute:" << endl;
		cout << "1. Selection Sort" << endl;
		cout << "2. Bubble Sort" << endl;
		cout << "3. Merge Sort" << endl;
		cout << "4. Insertion Sort" << endl;
		cout << "5. Heap Sort" << endl;
		cin >> num;

		time(&start);

		switch (num)
		{
		case 1:
			SelectionSort(arr);
			break;
		case 2:
			BubbleSort(arr);
			break;
		case 3:
			mergeSort(arr, 0, SIZE - 1);
			break;
		case 4:
			InsertionSort(arr);
			break;
		case 5:
			heapSort(arr);
			break;
		default:
			cout << "Invalid choice." << endl;
			break;
		}

		time(&end);
		// Calculating total time taken by the program. 
		double time_taken = double(end - start);
		std::cout << "Array size: " << SIZE << std::endl;
		std::cout << "Time taken by program is : " << std::fixed << time_taken << std::setprecision(5);
		std::cout << " sec " << std::endl;

		while (!quit)
		{
			quit = processEvent();
		}

		//Destroy renderer
		SDL_DestroyRenderer(renderer);
		//Destroy window
		SDL_DestroyWindow(window);
		//Quit SDL subsystems
		SDL_Quit();
	}

	return 0;
}
