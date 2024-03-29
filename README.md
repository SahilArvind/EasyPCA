# EasyPCA
A tool for running and visualising population genetics PCAs quickly. It works on both Linux and Windows. Use EasyPCA_lin.c/EasyPCA_lin.exe on Linux and EasyPCA_win.c/EasyPCA_win.exe on Windows. <br />

### Dependencies

- [Plink 1.90](https://www.cog-genomics.org/plink/)

### Installation

EasyPCA_win.c/EasyPCA_lin.c, which runs the PLINK PCA and prepares the input files for visualisation, is in the C/ directory. The visualisation process has to be done on R, using EasyPCA.R in the R/ directory. For convenience, store both EasyPCA_win.c/EasyPCA_lin.c and EasyPCA.R in your PLINK directory. <br />

On R, install the following libraries: <br />
``` r
install.packages("readr")
install.packages("tibble")
install.packages("dplyr")
install.packages("forcats")
install.packages("ggplot2")
install.packages("plotly")
install.packages("htmlwidgets")
```

### Usage

**EasyPCA_win.c/EasyPCA_lin.c**  <br />

This C code interacts with PLINK, a genetic analysis tool, to perform Principal Component Analysis (PCA) and generates two CSV files for downstream analysis and visualization:  <br />

 - PLINK Execution:  <br />
   * Takes user inputs for the number of principal components (numPCs) and dataset name.  <br />
   * Executes PLINK to perform PCA.  <br />

 - Data Processing and Output:  <br />
   * Reads PLINK output (plink.eigenvec) and writes relevant data to PCA_Data.csv.  <br />
   * Dynamically manages memory for unique populations, colors, and symbols.  <br />
   * Generates random aesthetics (colors and symbols) for populations.  <br />
   * Writes population aesthetics data to PCA_Aesthetics.csv.  <br />

 - Dynamic Memory Management  <br />
   * Uses dynamic memory allocation and reallocation as needed.  <br />

 - Symbol and Color Generation  <br />
   * Generates random hex colors and assigns unique symbols to each population.  <br />

 - Error Handling  <br />
   * Provides error messages for file-related issues and memory allocation failures.  <br />

 - Output Confirmation  <br />
   * Prints a success message upon successful generation of PCA_Data.csv and PCA_Aesthetics.csv.  <br />

*Execute the C program by running the following in the terminal (However, you also need to compile the program on Linux):* <br />
``` r
./EasyPCA_win.exe

        OR

gcc EasyPCA_lin.c -o EasyPCA_lin.exe       
./EasyPCA_lin.exe
```

**EasyPCA.R**  <br />

This R code uses various libraries to load and manipulate data, create a PCA scatter plot, and optionally save the plot or a zoomed version based on user inputs. The key steps are:  <br />

 - Libraries & Data  <br />
   * Load R libraries, read and join CSV files.  <br />

 - Data Manipulation  <br />
   * Convert, extract color and shape info.  <br />

 - User Input & Plot Creation:  <br />
   * Prompt for x, y, and title; create and display scatter plot.  <br />

 - Save Plot  <br />
   * Optionally saves the plot based on user preference, specifying format (pdf/png/html), file name, and dimensions.  <br />

 - Zoomed Plot  <br />
   * Optionally creates and saves a zoomed version of the plot, asking for X and Y axes limits and additional details. If requested, save the zoomed plot. <br />

 - Script Termination  <br />
   * Exits the script if the user chooses not to create a zoomed plot or exits at any point.  <br />

*Set the working directory and call EasyPCA.R by running the following:* <br />
``` r
setwd("path/to/plink/directory")
source("EasyPCA.R")
```