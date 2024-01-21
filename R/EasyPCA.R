# For loading files
library(readr)

# For a 'tidy' data frame or table format
library(tibble)

# For data manipulation
library(dplyr)

# For further data manipulation
library(forcats)

# For plotting
library(ggplot2)

# For interactive plots
library(plotly)

# For saving as HTML
library(htmlwidgets)

# Data Loading
data_pca <- read_csv("PCA_Data.csv", show_col_types = FALSE)
data_meta <- read_csv("PCA_Aesthetics.csv", show_col_types = FALSE)

data_combined <- left_join(data_pca, data_meta)

data_combined <- data_combined %>%
  mutate(Population = as_factor(Population))

list_colour <- data_combined %>% 
  select(Population, colorNr) %>% 
  deframe()

list_shape <- data_combined %>% 
  select(Population, symbolNr) %>% 
  deframe()

head(list_colour) 

# User input for x-axis
x_axis <- readline(prompt = "Enter the x-axis variable (e.g., PC1, PC2): ")

# User input for y-axis
y_axis <- readline(prompt = "Enter the y-axis variable (e.g., PC1, PC2): ")

# User input for plot title
plot_title <- readline(prompt = "Enter the title for the PCA plot: ")

gg <- ggplot(data = data_combined, 
             aes(x = !!rlang::sym(x_axis), y = !!rlang::sym(y_axis), colour = Population, shape = Population)) + 
  geom_point() +
  scale_colour_manual(values = list_colour) + 
  scale_shape_manual(values = list_shape) +
  theme(
    text = element_text(
      face = "bold",  # Set to "bold" to make the text bold
      size = 10       # Adjust the size of the text if needed
    )
  ) +
  ggtitle(plot_title)

# Print the plot
print(gg)

# Ask the user if they want to save the plot
save_plot <- tolower(readline(prompt = "Do you want to save the plot? (Yes/No): "))

# Variables to store information for saving the plot
file_format <- NULL
file_name <- NULL
units <- NULL
width <- NULL
height <- NULL

if (save_plot == "yes") {
  # Ask for the file format (pdf/png/html)
  file_format <- tolower(readline(prompt = "Enter the file format (pdf/png/html): "))

  # Ask for the filename
  file_name <- readline(prompt = "Enter the file name (without extension): ")

  # Ask for units, width, and height
  units <- readline(prompt = "Enter the units (e.g., in, cm): ")
  width <- as.numeric(readline(prompt = "Enter the width: "))
  height <- as.numeric(readline(prompt = "Enter the height: "))

  # Check if width and height are numeric
  if (!is.numeric(width) || !is.numeric(height)) {
    cat("Invalid input for width or height. Plot not saved.")
    q()
  }
}

# Save the initial plot
if (!is.null(file_format) && save_plot == "yes") {
  if (file_format %in% c("pdf", "png", "html")) {
    if (file_format %in% c("pdf", "png")) {
      ggsave(
        filename = paste0(file_name, ".", file_format),
        plot = gg,
        width = width,
        height = height,
        units = ifelse(!is.null(units), units, "in"),
        dpi = 600
      )
    } else if (file_format == "html") {
      my_widget <- ggplotly(gg)
      saveWidget(my_widget, file = paste0(file_name, ".html"))
    }
  } else {
    cat("Invalid file format. Plot not saved.")
  }
}

# Ask if the user wants a zoomed plot
zoom_plot <- tolower(readline(prompt = "Do you want to create a zoomed plot? (Yes/No): "))

if (zoom_plot == "yes") {
  # Ask for Y and X limits
  ylim_input <- readline(prompt = "Enter Y-axis limits (e.g., 0, 10): ")
  xlim_input <- readline(prompt = "Enter X-axis limits (e.g., -2, 2): ")

  # Convert input to numeric, handling potential errors
  ylim <- suppressWarnings(as.numeric(strsplit(ylim_input, ",")[[1]]))
  xlim <- suppressWarnings(as.numeric(strsplit(xlim_input, ",")[[1]]))

  # Check for missing or non-finite values
  if (any(is.na(ylim)) || any(is.na(xlim)) || any(!is.finite(c(ylim, xlim)))) {
    cat("Invalid input for limits. Exiting the script.\n")
    q()
  }

  # Create a zoomed plot
  gg_zoomed <- gg +
    coord_cartesian(ylim = ylim, xlim = xlim)

  # Print the zoomed plot
  print(gg_zoomed)

  # Ask if the user wants to save the zoomed plot
  save_zoomed_plot <- tolower(readline(prompt = "Do you want to save the zoomed plot? (Yes/No): "))

  if (save_zoomed_plot == "yes") {
    # Ask for the file format (pdf/png/html)
    file_format_zoomed <- tolower(readline(prompt = "Enter the file format (pdf/png/html): "))

    # Ask for the filename
    file_name_zoomed <- readline(prompt = "Enter the file name for the zoomed plot (without extension): ")

    # Ask for units, width, and height for the zoomed plot
    units_zoomed <- readline(prompt = "Enter the units for the zoomed plot (e.g., in, cm): ")
    width_zoomed <- as.numeric(readline(prompt = "Enter the width for the zoomed plot: "))
    height_zoomed <- as.numeric(readline(prompt = "Enter the height for the zoomed plot: "))

    # Check if width and height are numeric
    if (!is.numeric(width_zoomed) || !is.numeric(height_zoomed)) {
      cat("Invalid input for width or height. Zoomed plot not saved.")
      q()
    }

    # Save the zoomed plot
    if (file_format_zoomed %in% c("pdf", "png", "html")) {
      if (file_format_zoomed %in% c("pdf", "png")) {
        ggsave(
          filename = paste0(file_name_zoomed, ".", file_format_zoomed),
          plot = gg_zoomed,
          width = width_zoomed,
          height = height_zoomed,
          units = units_zoomed,
          dpi = 600
        )
      } else if (file_format_zoomed == "html") {
        my_widget_zoomed <- ggplotly(gg_zoomed)
        saveWidget(my_widget_zoomed, file = paste0(file_name_zoomed, ".html"))
      }
    } else {
      cat("Invalid file format. Zoomed plot not saved.")
    }
  } else {
    cat("Zoomed plot not saved.")
  }
} else {
  cat("Exiting the script.\n")
}