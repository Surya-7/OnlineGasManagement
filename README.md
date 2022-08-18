# OnlineGasManagement

This is a data structure project to To create an online gas management portal for customer and distributor, such that to store all the necessary details of the 
customer and distributor. To provide options for the customers to purchase different products (LPG, Regulator, Lighter) from the nearest distributor who has the 
adequate stock to deliver the products to the customer. Also generate bill amount of the purchases made and display to the user.

Customer: For customers it asks whether to sign in or sign up. In case of sign up it collects all the details of the customer and generates a unique customer id 
for all the customers and using that customer id the customer nodes are inserted in the AVL trees. In case of sign in it requires the customer id using which the 
customer could sign in. Using the customer id it searches if the id is present in the AVL tree. If the customer id is not present then a message is given to the 
user that it is not found. After successful sign in or sign up, it asks to place an order or to delete the customer details. For placing an order it collects the 
details of theproducts and quantity from the user. Using the floyd-warshall algorithm we assign the nearest distributor to the customer who has adequate stock to 
supply the customer and after purchase maintain the new stocks of the distributor and generate the bill to the customer.

Distributor: For distributors it asks to add a new distributor. To add a new distributor we need the personal details of the distributor and also collect the 
stock availability of all the products (LPG, Regulator, Lighter). Store all the distributor nodes in a vector.
