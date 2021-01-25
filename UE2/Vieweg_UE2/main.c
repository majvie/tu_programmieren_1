#include <stdio.h>
#include <math.h>

/*
  Ich habe leider die macOs Version Catalina und habe nicht die Moeglichkeit gehabt
  die OS zu aendern, daher konnte ich Codeblocks nicht verwenden.
*/

int main()
{
  double result, operand, resultRounded;
  int precision = 0;
  char operator = 0;

  // Input
  printf("Operand: ");
  if (scanf("%lf", &result) != 1) {
    printf("Invalid Argument: Operand must be a number");
    return -1;
  }

  while (1) {
    printf("Operator: ");
    scanf(" %c", &operator); // space before %c because of \n

    // Validation
    switch(operator)
    {
      case '.':
        printf("=========\nProgrammende");
        return 1;
      case 'c':
        if (result < 0 || result > 360) {
          printf("Invalid Argument: Operand out of range");
          return -1;
        } break;
      case 'l':
        if (result < 0) {
          printf("Invalid Argument: Operand out of range");
          return -1;
        } break;
      case 'r':
        printf("Nachkommastellen: ");
        if (scanf("%d", &precision) != 1) {
          printf("Invalid Argument: Operand must be a number");
        }
        if (precision < 0) {
          printf("Precision out of range");
          return -1;
        }
        break;
      case '+':
      case '-':
      case '*':
      case 'p':
      case '/':
        printf("Operand: ");
        if (scanf("%lf", &operand) != 1) {
          printf("Invalid Argument: Operand must be a number");
          return -1;
        }
        if (operator == '/' && operand == 0) {
          printf("Invalid Argument: Division by zero not possible.");
          return -1;
        }
        if (operator == 'p') {
          if (result < 0 && floor(operand) != operand) {
            printf("Invalid Argument: Operand out of range.");
            return -1;
          }
          else if (result == 0 && operand == 0) {
            printf("Invalid Argument: Operand out of range.");
            return -1;
          }
        }
        break;
      default:
        printf("Invalid Argument: Operator not defined.");
        return -1;
    }

    // Calculation
    switch(operator)
    {
      case '+': result = result + operand; break;
      case '-': result = result - operand; break;
      case '*': result = result * operand; break;
      case 'p': result = pow(result, operand); break;
      case 'l': result = log(result); break;
      case 'c': result = cos(result / 180.0 * M_PI); break;
      case '/': result = result / operand; break;
      case 'r': {
        double scalingFactor = pow(10, precision);
        resultRounded = floor(result*scalingFactor) / scalingFactor;
      } break;
    }

    printf("=========\n");
    if (operator == 'r') {
      printf("Ergebnis: %.*f\n\n", precision + 3, resultRounded);
    } else {
      printf("Ergebnis: %g\n\n", result);
    }
  }
}
