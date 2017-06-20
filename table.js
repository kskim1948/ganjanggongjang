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

ex_data=500;
app.get('/', function(req,res){
        if(req.query.temp && typeof req.query.temp!='undefined'){
                var dt = date.create();
                var formatted_date = dt.format('Y-m-d');
                var formatted_time = dt.format('H:M');
                res.send(formatted_date+' Temp:'+req.query.temp);
                console.log(formatted_date+' Temp:'+req.query.temp);

                fs.appendFile('LOG_table.txt',req.query.temp+'\n', function(err){
                        if(err) throw err;
                });

                data={};
                data.date=formatted_date;
                data.timer=formatted_time;
                timer_str = req.query.temp;

                timer_int=parseInt(timer_str)/1000;
                h=parseInt(timer_int/3600);
                m=parseInt(((timer_int-parseInt(parseInt(timer_int/3600)*3600))/60));

                data.time=h+':'+m;
                console.log(data.time);
                if(ex_data>timer_int){
                        console.log('22222222222222222');
                        connection.query('INSERT INTO table1 SET ?',data,function(err,rows,cols){
                                if(err) throw err;

                                console.log('Done Insert Query');
                        });
                        console.log('333333333333333');
                }
                else
                {
                        console.log('4444444444444');
                        connection.query('UPDATE table1 SET time = ? ORDER BY timer DESC LIMIT 1',data.time,function(err,rows,cols){
                                if(err) throw err;

                                console.log('Done Update Query');
                        });
                        console.log('555555555555555');
                }

                ex_data=timer_int;
        }
         else{
                res.send('Unauthorized Access');
        }
})

app.get('/dump',function(req,res){
        console.log('66666666666666');
        connection.query('SELECT * from table1',function(err,rows,cols){
                if(err) throw err;

                res.write('<html><head><title>This is the CAS</title></head><body>');
                res.write('<p>CAS</p>');
                var dt = date.create();
                var formatted = dt.format('Y-m-d H:M:S');
                //res.write('<p>Dump '+rows.length+' data at '+formatted+'</p>');
                //Send HTML table
                res.write('<table border="3">');
                res.write('<tr><th>Date</th><th>Time</th><th>Table_Num</th><th>Occupied_Time</th></tr>');
                for(var i=0;i<rows.length;i++){
                        var row=rows[i];
                        res.write('<tr>');
                        res.write('<td>'+row.date+'</td><td>'+row.timer+'</td><td>'+'1'+'</td><td>'+row.time+'</td>');
                        res.write('</tr>');
                }
                res.end('</table></body></html>');
      		console.log('Complete');
        });
})

app.listen(8080, function(){
        console.log('scale measuring start')
})
