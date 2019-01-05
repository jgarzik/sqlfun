create table if not exists TheTableName (
	id int(11) not null auto_increment primary key,
	region varchar(32) not null,
	data_size bigint not null default '0',
	createtime datetime not null
);
