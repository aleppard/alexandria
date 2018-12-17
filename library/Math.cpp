// The "export" syntax is part of C++ Module TS. It is not standard C++
// and clang's module TS support does not appear to be fully supported
// by the clang toolchain.

// export module Math;

/* export */ long fibonacci(long number) {

  // This implementation is for demonstration purposes only and is not
  // meant to be an industry standard implementation. It does not provide
  // proper overflow detection and there are faster implementations available.
  long previous = 1L;
  long current = 1L;
  long next = 1L;
  
  for (long i = 3L; i <= number; i++) {
    next = current + previous;
    previous = current;
    current = next;
  }
  
  return next;
}
