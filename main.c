#include <iostream>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#include <unistd.h> 

#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

void takeScreenshot(char *array, unsigned int width, unsigned int height, XImage *image)
{


   unsigned long red_mask = image->red_mask;
   unsigned long green_mask = image->green_mask;
   unsigned long blue_mask = image->blue_mask;

   for (int x = 0; x < width; x++)
      for (int y = 0; y < height ; y++)
      {
         unsigned long pixel = XGetPixel(image,x,y);

         unsigned char blue = pixel & blue_mask;
         unsigned char green = (pixel & green_mask) >> 8;
         unsigned char red = (pixel & red_mask) >> 16;

         array[(x + width * y) * 3] = red;
         array[(x + width * y) * 3+1] = green;
         array[(x + width * y) * 3+2] = blue;
      }
}

int main()
{
    std::cout << "Hello" << std::endl;
Display *display = XOpenDisplay(NULL);
if (display == nullptr)
{
	printf("fail");
	exit(1);
}
   Window root = DefaultRootWindow(display);

   XWindowAttributes gwa;

   XGetWindowAttributes(display, root, &gwa);
   int width = gwa.width;
   int height = gwa.height;
   
   printf("Width %d, Height %d\n", width, height);


   XImage *image = XGetImage(display,root, 0,0 , width,height,AllPlanes, ZPixmap);

	unsigned int imgSize = width * height * 3;
	unsigned char *array = new unsigned char[imgSize];

   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 4148;
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	
   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }
   
   /* If connection is established then start communicating */
   bzero(buffer, 256);
   n = read( newsockfd,buffer,255 );
   
   if (n < 0) {
      perror("ERROR reading from socket");
   
      exit(1);
   }
   
   printf("Here is the message: %s\n",buffer);
   bool alive = true;
   char aliveMsg;
   while (alive)
   {
		/*n = read( newsockfd,&aliveMsg,1 );
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		alive = (bool)aliveMsg;*/
	/* Write a response to the client */
	
//	n = write(newsockfd, &imgSize, (int)sizeof(imgSize));
image = XGetImage(display, root, 0,0 , width,height,AllPlanes, ZPixmap);

	takeScreenshot(array, width, height, image);
	n = write(newsockfd,array,640*480*3);
	if (n < 0) {
		perror("ERROR writing to socket");
		exit(1);
	}
	   
   }
   
      
    return 0;
}


