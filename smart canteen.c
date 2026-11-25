/* SMART CANTEEN - With File Handling (PPS Course Based Project)
 *
 * Files used (all plain text, created automatically in the program folder):
 *   users.txt    - registered usernames and passwords
 *   stock.txt    - current stock of every food item
 *   sales.txt    - token counter, total orders, total sales, rating totals
 *   orders.txt   - history of every paid order (append only)
 *   feedback.txt - every rating and comment given (append only)
 */

#include <stdio.h>
#include <string.h>

/* ---------- File names ---------- */
#define USERS_FILE    "users.txt"
#define STOCK_FILE    "stock.txt"
#define SALES_FILE    "sales.txt"
#define ORDERS_FILE   "orders.txt"
#define FEEDBACK_FILE "feedback.txt"

/* ---------- Food data (parallel arrays) ---------- */
int   id[10]       = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
char  name[10][20] = {"Idli", "Dosa", "Meals", "Biryani", "Samosa",
                      "Puff", "Sandwich", "Tea", "Coffee", "Juice"};
int   category[10] = {1, 1, 2, 2, 3, 3, 3, 4, 4, 4};   /* 1=Breakfast 2=Lunch 3=Snacks 4=Beverages */
float price[10]    = {30, 50, 80, 90, 15, 20, 40, 10, 15, 35};
int   stock[10]    = {20, 15, 25, 20, 40, 30, 18, 50, 50, 20};

/* ---------- Order data ---------- */
int orderQty[10] = {0};     /* quantity ordered for each food item */
int tokenCounter = 100;

/* ---------- User data ---------- */
char usernames[20][20];
char passwords[20][20];
int  userCount = 0;

/* ---------- Sales report data ---------- */
int   totalOrders = 0;
float totalSales  = 0;
int   ratingSum   = 0;
int   ratingCount = 0;

/* ===================================================== */
/* ================= FILE HANDLING ===================== */
/* ===================================================== */

/* ----- Users ----- */
void saveUsers() {
    int i;
    FILE *fp = fopen(USERS_FILE, "w");
    if (fp == NULL) {
        printf("Warning: could not save users file!\n");
        return;
    }
    for (i = 0; i < userCount; i++) {
        fprintf(fp, "%s %s\n", usernames[i], passwords[i]);
    }
    fclose(fp);
}

void loadUsers() {
    FILE *fp = fopen(USERS_FILE, "r");
    if (fp == NULL) return;          /* first run: no file yet */
    userCount = 0;
    while (userCount < 20 &&
           fscanf(fp, "%19s %19s", usernames[userCount], passwords[userCount]) == 2) {
        userCount++;
    }
    fclose(fp);
}

/* ----- Stock ----- */
void saveStock() {
    int i;
    FILE *fp = fopen(STOCK_FILE, "w");
    if (fp == NULL) {
        printf("Warning: could not save stock file!\n");
        return;
    }
    for (i = 0; i < 10; i++) {
        fprintf(fp, "%d %d\n", id[i], stock[i]);
    }
    fclose(fp);
}

void loadStock() {
    int fid, qty;
    FILE *fp = fopen(STOCK_FILE, "r");
    if (fp == NULL) return;          /* first run: keep default stock */
    while (fscanf(fp, "%d %d", &fid, &qty) == 2) {
        if (fid >= 1 && fid <= 10) {
            stock[fid - 1] = qty;
        }
    }
    fclose(fp);
}

/* ----- Sales report data ----- */
void saveSales() {
    FILE *fp = fopen(SALES_FILE, "w");
    if (fp == NULL) {
        printf("Warning: could not save sales file!\n");
        return;
    }
    fprintf(fp, "%d %d %.2f %d %d\n",
            tokenCounter, totalOrders, totalSales, ratingSum, ratingCount);
    fclose(fp);
}

void loadSales() {
    FILE *fp = fopen(SALES_FILE, "r");
    if (fp == NULL) return;          /* first run: keep defaults */
    fscanf(fp, "%d %d %f %d %d",
           &tokenCounter, &totalOrders, &totalSales, &ratingSum, &ratingCount);
    fclose(fp);
}

/* ----- Order history (append one record per paid order) ----- */
void saveOrderHistory(float total) {
    int i;
    FILE *fp = fopen(ORDERS_FILE, "a");
    if (fp == NULL) {
        printf("Warning: could not save order history!\n");
        return;
    }
    fprintf(fp, "Token T%d | ", tokenCounter);
    for (i = 0; i < 10; i++) {
        if (orderQty[i] > 0) {
            fprintf(fp, "%s x%d  ", name[i], orderQty[i]);
        }
    }
    fprintf(fp, "| Total Rs.%.2f\n", total);
    fclose(fp);
}

/* ----- Feedback log (append) ----- */
void saveFeedback(int rating, char comment[]) {
    FILE *fp = fopen(FEEDBACK_FILE, "a");
    if (fp == NULL) {
        printf("Warning: could not save feedback!\n");
        return;
    }
    fprintf(fp, "Rating: %d | Comment: %s\n", rating, comment);
    fclose(fp);
}

/* ----- Load everything at startup ----- */
void loadAllData() {
    loadUsers();
    loadStock();
    loadSales();
    /* create the files on first run so they always exist */
    saveStock();
    saveSales();
    saveUsers();
}

/* ===== MODULE 1: Registration and Login ===== */
void registerUser() {
    int i;
    char newName[20];

    if (userCount >= 20) {
        printf("User limit reached!\n");
        return;
    }
    printf("Enter new username: ");
    scanf("%19s", newName);

    /* do not allow duplicate usernames */
    for (i = 0; i < userCount; i++) {
        if (strcmp(newName, usernames[i]) == 0) {
            printf("Username already exists! Try another.\n");
            return;
        }
    }

    strcpy(usernames[userCount], newName);
    printf("Enter new password: ");
    scanf("%19s", passwords[userCount]);
    userCount++;
    saveUsers();                         /* store in file */
    printf("Registration successful!\n");
}

int loginUser() {
    char u[20], p[20];
    int i;
    printf("Enter username: ");
    scanf("%19s", u);
    printf("Enter password: ");
    scanf("%19s", p);
    for (i = 0; i < userCount; i++) {
        if (strcmp(u, usernames[i]) == 0 && strcmp(p, passwords[i]) == 0) {
            printf("Login successful! Welcome %s\n", u);
            return 1;
        }
    }
    printf("Wrong username or password!\n");
    return 0;
}

/* ===== MODULE 2: Food Menu ===== */
void showMenu() {
    int i;
    printf("\nID  Item        Price     Stock Left\n");
    printf("-------------------------------------\n");
    for (i = 0; i < 10; i++) {
        /* stock left = stock in kitchen - quantity already in your order */
        printf("%-3d %-10s  Rs.%-5.0f  %d\n", id[i], name[i], price[i], stock[i] - orderQty[i]);
    }
}

/* ===== Update Stock (restock an item) ===== */
void updateStock() {
    int foodId, addQty;
    showMenu();
    printf("Enter food ID to restock: ");
    scanf("%d", &foodId);
    if (foodId < 1 || foodId > 10) {
        printf("Invalid food ID!\n");
        return;
    }
    printf("Enter quantity to add: ");
    scanf("%d", &addQty);
    if (addQty <= 0) {
        printf("Quantity must be more than 0!\n");
        return;
    }
    stock[foodId - 1] += addQty;
    saveStock();                         /* store in file */
    printf("%s stock updated. New stock: %d\n", name[foodId - 1], stock[foodId - 1]);
}

/* ===== MODULE 3: Food Categories ===== */
void showCategory() {
    int c, i;
    printf("1.Breakfast  2.Lunch  3.Snacks  4.Beverages\n");
    printf("Enter category: ");
    scanf("%d", &c);
    printf("\nID  Item        Price\n");
    for (i = 0; i < 10; i++) {
        if (category[i] == c) {
            printf("%-3d %-10s  Rs.%.0f\n", id[i], name[i], price[i]);
        }
    }
}

/* ===== MODULE 7: Food Availability ===== */
int isAvailable(int index, int qty) {
    if (stock[index] >= qty + orderQty[index]) {
        return 1;
    }
    return 0;
}

/* ===== MODULE 4 + 5: Food Selection and Quantity ===== */
void selectFood() {
    int foodId, qty;
    showMenu();
    printf("Enter food ID: ");
    scanf("%d", &foodId);
    if (foodId < 1 || foodId > 10) {
        printf("Invalid food ID!\n");
        return;
    }
    printf("Enter quantity: ");
    scanf("%d", &qty);
    if (qty <= 0) {
        printf("Quantity must be more than 0!\n");
        return;
    }
    if (isAvailable(foodId - 1, qty)) {
        orderQty[foodId - 1] += qty;
        printf("%d %s added to your order.\n", qty, name[foodId - 1]);
    } else {
        printf("Sorry, not enough %s available!\n", name[foodId - 1]);
    }
}

/* ===== MODULE 6: Order Management ===== */
void viewOrder() {
    int i, empty = 1;
    printf("\n--- Your Order ---\n");
    for (i = 0; i < 10; i++) {
        if (orderQty[i] > 0) {
            printf("%s x %d = Rs.%.0f\n", name[i], orderQty[i], price[i] * orderQty[i]);
            empty = 0;
        }
    }
    if (empty) {
        printf("No items in order.\n");
    }
}

void modifyOrder() {
    int foodId, newQty;
    viewOrder();
    printf("Enter food ID to modify: ");
    scanf("%d", &foodId);
    if (foodId < 1 || foodId > 10 || orderQty[foodId - 1] == 0) {
        printf("That item is not in your order!\n");
        return;
    }
    printf("Enter new quantity (0 to remove): ");
    scanf("%d", &newQty);
    if (newQty < 0 || newQty > stock[foodId - 1]) {
        printf("Invalid quantity!\n");
        return;
    }
    orderQty[foodId - 1] = newQty;
    printf("Order updated.\n");
}

void cancelOrder() {
    int i;
    for (i = 0; i < 10; i++) {
        orderQty[i] = 0;
    }
    printf("Order cancelled.\n");
}

/* ===== MODULE 8: Bill Calculation ===== */
float calculateBill() {
    int i;
    float subtotal = 0, discount = 0, tax, total;

    for (i = 0; i < 10; i++) {
        subtotal += price[i] * orderQty[i];
    }

    if (subtotal >= 500) {
        discount = subtotal * 0.10;      /* 10% discount */
    } else if (subtotal >= 200) {
        discount = subtotal * 0.05;      /* 5% discount */
    }

    tax   = (subtotal - discount) * 0.05;   /* 5% GST */
    total = subtotal - discount + tax;

    printf("\n------ BILL ------\n");
    printf("Subtotal : Rs. %.2f\n", subtotal);
    printf("Discount : Rs. %.2f\n", discount);
    printf("GST 5%%   : Rs. %.2f\n", tax);
    printf("TOTAL    : Rs. %.2f\n", total);
    printf("------------------\n");
    return total;
}

/* ===== MODULE 9: Payment and Token Generation ===== */
void payment() {
    int i, mode, empty = 1;
    float total;

    for (i = 0; i < 10; i++) {
        if (orderQty[i] > 0) empty = 0;
    }
    if (empty) {
        printf("Your order is empty!\n");
        return;
    }

    viewOrder();
    total = calculateBill();

    printf("Payment mode: 1.Cash  2.UPI  3.Card : ");
    scanf("%d", &mode);
    if (mode < 1 || mode > 3) {
        printf("Payment failed!\n");
        return;
    }

    /* payment success: update counters first so the token is correct */
    tokenCounter++;
    totalOrders++;
    totalSales += total;

    /* save order history while the order items are still available */
    saveOrderHistory(total);

    /* reduce stock and clear order */
    for (i = 0; i < 10; i++) {
        stock[i] -= orderQty[i];
        orderQty[i] = 0;
    }

    saveStock();                         /* store updated stock */
    saveSales();                         /* store token + sales totals */

    printf("\nPayment Status : SUCCESS\n");
    printf("Your Token No  : T%d\n", tokenCounter);
}

/* ===== MODULE 10: Feedback and Report ===== */
void feedback() {
    int rating;
    char comment[100];
    printf("Rate us (1-5): ");
    scanf("%d", &rating);
    if (rating < 1 || rating > 5) {
        printf("Invalid rating!\n");
        return;
    }
    printf("Your comment (one word): ");
    scanf("%99s", comment);
    ratingSum += rating;
    ratingCount++;
    saveFeedback(rating, comment);       /* store comment in file */
    saveSales();                         /* store rating totals */
    printf("Thank you for your feedback!\n");
}

void salesReport() {
    printf("\n----- SALES REPORT -----\n");
    printf("Total orders : %d\n", totalOrders);
    printf("Total sales  : Rs. %.2f\n", totalSales);
    if (ratingCount > 0) {
        printf("Average rating: %.1f / 5\n", (float)ratingSum / ratingCount);
    } else {
        printf("No feedback yet.\n");
    }
}

/* ===== Customer Menu ===== */
void customerMenu() {
    int choice;
    do {
        printf("\n===== SMART CANTEEN =====\n");
        printf("1. View Menu\n");
        printf("2. View by Category\n");
        printf("3. Select Food\n");
        printf("4. View Order\n");
        printf("5. Modify Order\n");
        printf("6. Cancel Order\n");
        printf("7. Pay Bill & Get Token\n");
        printf("8. Give Feedback\n");
        printf("9. Sales Report\n");
        printf("10. Update Stock\n");
        printf("0. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: showMenu();     break;
            case 2: showCategory(); break;
            case 3: selectFood();   break;
            case 4: viewOrder();    break;
            case 5: modifyOrder();  break;
            case 6: cancelOrder();  break;
            case 7: payment();      break;
            case 8: feedback();     break;
            case 9: salesReport();  break;
            case 10: updateStock(); break;
            case 0: printf("Logged out.\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
}

/* ===== Main ===== */
int main() {
    int choice;

    loadAllData();                       /* read saved data from files at startup */

    printf("**** WELCOME TO SMART CANTEEN ****\n");
    do {
        printf("\n1. Register\n2. Login\n0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: registerUser(); break;
            case 2:
                if (loginUser()) {
                    customerMenu();
                }
                break;
            case 0: printf("Thank you! Visit again.\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
    return 0;
}
