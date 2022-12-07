# Statistic-Bots
## Explore network programming, a TCP/IP client that communicates with a remote process in  <a href=https://www-edlab.cs.umass.edu target="blank">UMass Amherst Manning CICS Laboratory</a>

- Implemented a network client that communicates to the remote server in the C programming language
- The client then solve a series of simple statistics problems, finding min/max/median number among 5 numbers.
- The steps are outlined as follows:
  - Open a TCP stream socket
  - Connect to the remote server on the specified port number
  - Send SPIRE ID to the server in the following format: [NETID]@umass.edu
  - Receive the first statistics problem
  - Send the CORRECT solution to the server (Server will drop connection if it is wrong)
  - Continue steps 4 and 5 some number of times (fewer than 2000)
  - Once step 6 is completed, you will receive a 64-byte string (“flag”) that is unique to your NetID. Once you “capture the flag” you know you have implemented your client correctly.

- Test server is at address 128.119.243.147 (this is elnux1) on port 27994

- Compiling command: gcc client.c -o client
- Running command: .client [NETID]@umass.edu 27994 128.119.243.147
