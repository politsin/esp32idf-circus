idf_component_register(
  SRCS
  "main.cpp"
  "lib/DendoStepper.cpp"
  "task/blinkTask.cpp"
  "task/buttonTask.cpp"
  "task/encoderTask.cpp"
  "task/stepperTask.cpp"
  INCLUDE_DIRS "util" "task" "lib"
)

