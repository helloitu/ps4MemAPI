//Payload desc
//http://ps4ip:771/list - grabs process list
//http://ps4ip:771/mapping?pid=X - grabs memory mapping for process
//http://ps4ip:771/write?pid=X&address=X&data=X - write memory (data is base64)
//http://ps4ip:771/read?pid=X&address=X&length=X - read memory
//http://ps4ip:771/alloc?pid=X&length=X - allocate memory (this is RWX)
//http://ps4ip:771/free?pid=X&address=X&length=X - free memory
//The responses are in json

function getrequest(url, callback) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() {
        callback(xmlHttp.responseText);
    }
    xmlHttp.open("GET", url, true);
    xmlHttp.send(null);
}

function PS4WebRTE(ps4ip) {
    this.ip = ps4ip;

    this.list = function(callback) {
        getrequest("http://" + this.ip + ":771/list", callback);
    }
    
    this.mapping = function(pid, callback) {
        getrequest("http://" + this.ip + ":771/mapping?pid=" + pid, callback);
    }

    this.write = function(pid, address, data, length, callback) {
        getrequest("http://" + this.ip + ":771/write?pid=" + pid + "&address=" + address + "&data=" + btoa(data) + "&length=" + length, callback);
    }

    this.read = function(pid, address, length, callback) {
        getrequest("http://" + this.ip + ":771/read?pid=" + pid + "&address=" + address + "&length=" + length, callback);
    }

    this.alloc = function(pid, length, callback) {
        getrequest("http://" + this.ip + ":771/alloc?pid=" + pid + "&length=" + length, callback);
    }

    this.free = function(pid, address, length, callback) {
        getrequest("http://" + this.ip + ":771/free?pid=" + pid + "&address=" + address + "&length=" + length, callback);
    }
}