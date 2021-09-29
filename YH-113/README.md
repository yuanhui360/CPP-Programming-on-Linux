 /*
  *  Headers Exchange
  *  A headers exchange is designed for routing messages based on multiple header attributes
  *  instead of routing key
  *
  *   1. No routing key required
  *   2. The header attributes format is <key:value>
  *   3. The special argument named "x-match" can have two different values: "any" or "all",
  *      which specify if all headers must match or just one., default value is "all".
  *   4. Headers Exchange is more flexable as it could be an integer or a hash (dictionary)
  *      (it does not have to be a string as the routing key)
  *
  */

Video link of this example : https://youtu.be/YxlxdzkG3ds
