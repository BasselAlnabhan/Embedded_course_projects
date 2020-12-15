# A Secure and Safe Communication Between a Client and a Server

## Authentication Steps

    1. Client generates an 8 bytes random token

    2. Client signs and encrypts the token and sends it to the server
    
    3. Server receives the token and generates a random AES key
    
    4. Server signs and encrypts the key and sends it to the client
    
    5. Client receives the AES key and encrypts the token by the AES key and sends it to the server
  
    6. Server receives the the token and comapares it with the token from step 3
    
    7. If the tokens match, server generates an 8 bytes random responseID and encrypts it by the AES key and sends it to the client
    
    8. Client receives the responseID and uses it to initialize its requestID
    
    9. Client increaments its requestID for each sent request and the server increaments its responsetID for each response
    
    10. For each request/response, requestID and responseID shall match.
