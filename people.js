var express = require('express')
var app = express()
var fs = require('fs')

var mysql = require('mysql')
var connection = mysql.createConnection({
        host : 'localhost',
        user : 'CAS',
        password : 'YAPyap30!',
        database : 'scale'
});

connection.connect();

var date = require('node-datetime');

var os = require('os');
var interfaces = os.networkInterfaces();
var addresses = [];
for (var k in interfaces) {
    for (var k2 in interfaces[k]) {
        var address = interfaces[k][k2];
        if (address.family === 'IPv4' && !address.internal) {
          addresses.push(address.address);
        }
    }
}


count=0;
ex_date='0000';
app.get('/', function(req,res){
        if(req.query.temp && typeof req.query.temp!='undefined'){
                var dt = date.create();
                var formatted_date = dt.format('Y-m-d');
                var formatted_time = dt.format('H:M');
                res.send(formatted_date+' Temp:'+req.query.temp);
                console.log(formatted_date+' Temp:'+req.query.temp);

                fs.appendFile('LOG.txt',req.query.temp+'\n', function(err){
                        if(err) throw err;
                });

                data={};
                data.count = req.query.temp;
                data.date = formatted_date;
                data.time = formatted_time;
//              if(ex_date != formatted_date)
//              {
                connection.query('INSERT INTO p_count SET ?',data,function(err,rows,cols){
                        if(err) throw err;

                        console.log('Done Insert Query');
                        console.log(formatted_time);
                });
  //                    }
        /*      else
                {
                        connection.query('UPDATE p_count SET count = ? WHERE date = ?',[data.count,data.date],function(err,rows,cols){
                                if(err) throw err;
                                console.log('Done Update Query');
                        });
                }*/
                ex_date=formatted_date;
        }
        else{
                res.send('Unauthorized Access');
        }
})

app.get('/dump',function(req,res){

        connection.query('SELECT * from p_count',function(err,rows,cols){
                if(err) throw err;
res.write('<html><head><title>This is the CAS</title></head><body>');
                res.write('<p>CAS</p>');
                var dt = date.create();
                var formatted = dt.format('Y-m-d H:M:S');
                res.write('<p>Dump '+rows.length+' data at '+formatted+'</p>');
                //Send HTML table
                res.write('<table border="3">');
                res.write('<tr><th>Date</th><th>Time</th><th>Count</th></tr>');
                for(var i=0;i<rows.length;i++){
                        var row=rows[i];
                        res.write('<tr>');
                        res.write('<td>'+row.date+'</td><td>'+row.time+'</td><td>'+row.count+'</td>');
                        res.write('</tr>');
                }
                res.end('</table></body></html>');
                console.log('Complete');
        });
})

app.listen(3000, function(){
        console.log('scale measuring start')
})

                                                                       