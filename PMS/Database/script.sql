USE PHARMACY;
				CREATE DATABASE PHARMACY;
GO

-------------------------------------------------------------STOCK------------------------------------------------
CREATE TABLE Stocks
(
	Id bigint not null,
	"Name" varchar(50) not null,
	"Status" varchar(50) not null,
	Quantity_Status varchar(6) not null,
	Quantity int not null,
	Cost_Price decimal(3, 3) not null,
	Sole_Price decimal(3, 3) not null,
	"User" varchar(12) not null,
	MFG_date datetime not null,
	"Description" varchar(300) not null,
	Photo VARBINARY,
	Exp_date datetime not null,
	Supplier varchar(50) not null,
	Supplier_Contact v		archar(50) not null,
	"Date Added" datetime,
	constraint pk_Id PRIMARY KEY(Id),
	constraint chk check(Quantity > 0 and Sole_Price > 0 and Cost_Price > 0),
    constraint uniqu_name unique("Name"),
)
GO

CREATE INDEX stock_index ON Stocks("Name");--Creating index to facilitate searching of drug name

-- Create a new stored procedure called 'StoredProcedureName' in schema 'dbo'
-- Drop the stored procedure if it already exists
IF EXISTS
(
	SELECT * FROM INFORMATION_SCHEMA.ROUTINES
	WHERE SPECIFIC_SCHEMA = N'dbo' AND SPECIFIC_NAME = N'MyStock' AND ROUTINE_TYPE = N'PROCEDURE'
)
DROP PROCEDURE dbo.StoredProcedureName
GO

CREATE PROCEDURE MyStock
(
	@Id bigint,
	@Name varchar(50),
	@Status varchar(50),
	@Quantity_Status varchar(6),
	@Quantity int,
	@Cost_Price decimal,
	@Sole_Price decimal,
	@User varchar(12),
	@MFG_date datetime,
	@Exp_date datetime,
	@Description varchar(300),
	@Photo VARBINARY,
	@Supplier varchar(50),
	@Supplier_Contact varchar(50),
	@Date_Added datetime
)AS
BEGIN
	BEGIN TRY
		BEGIN TRANSACTION;

		 DECLARE @stocks nvarchar(1000);
        SET @stocks = N'INSERT INTO Stocks (Id, "Name", Status, Quantity_Status, Quantity, Cost_Price, Sole_Price, User, MFG_date, Exp_date, "Description", Photo, Supplier, Supplier_Contact, "Date Added")
		VALUES (@Id, @"Name", @"Status", @Quantity_Status, @Quantity, @"User", @Cost_Price, @Sole_Price, @"Description", @MFG_date, @Exp_date, @Supplier, @Supplier_Contact, @Photo, @"Date Added")';
		
		COMMIT TRANSACTION;
	END TRY

	BEGIN CATCH
		IF @@TRANCOUNT > 0
			ROLLBACK TRANSACTION;

			DECLARE @error VARCHAR(400) = ERROR_MESSAGE();
			DECLARE @severe INT = ERROR_SEVERITY();
			DECLARE @condition INT = ERROR_STATE();

			RAISERROR (@error, @severe, @condition);
	END CATCH
END
GO
-------------------------------------------------------------ADD USER------------------------------------------------
CREATE TABLE ADD_USER
(
	Id bigint not null,
	"Name" varchar(50),
	"Status" varchar(12) not null,
	Email varchar(20) not null,
	"Password" varchar(20) not null,
	Photo VARBINARY,
	Issued_Date datetime,
	constraint pk_Idx primary key(Id),
    constraint unique_email unique(Email),
	constraint chk_email_format check(Email like '%_@__%.__%')
);
GO
CREATE INDEX User_Index on ADD_USER("Name");

-- Create a new stored procedure called 'StoredProcedureName' in schema 'dbo'
-- Drop the stored procedure if it already exists
IF EXISTS
(
	SELECT * FROM INFORMATION_SCHEMA.ROUTINES
	WHERE SPECIFIC_SCHEMA = N'dbo' AND SPECIFIC_NAME = N'MyUser' AND ROUTINE_TYPE = N'PROCEDURE'
)
DROP PROCEDURE dbo.StoredProcedureName

GO
CREATE PROCEDURE MyUser
(
	@Id bigint,
	@Name varchar(50),
	@Status varchar(12),
	@Email varchar(20),
	@Password varchar(20),
	@Photo VARBINARY,
	@Issued_Date datetime
)AS
BEGIN
	BEGIN TRY
		BEGIN TRANSACTION;

		 DECLARE @users nvarchar(1000);
        SET @users = N'INSERT INTO Stocks (Id, "Name", "Status", Email, "Password", Photo, Issued_Date)
		VALUES (@Id, @Name, @Status, @Email, @Password, @Photo, @Issued_Date)';

		EXEC Sp_executesql @users, N'@Id, @Name, @Status, @Email, @Password, @Photo, @Issued_Date',
		@Id, @Name, @Status, @Email, @Password, @Photo, @Issued_Date;

	--	EXEC MyUser @Id = 10005, @Name = 'SmartBrain', @Status = 'Admin', @Email = 'smart@gmail.com', @Password = 'smart@35', @Photo =  null, @Issued_Date = null;

		COMMIT TRANSACTION;
	END TRY

	BEGIN CATCH
		IF @@TRANCOUNT > 0
			ROLLBACK TRANSACTION;

			DECLARE @errors VARCHAR(400) = ERROR_MESSAGE();
			DECLARE @severity INT = ERROR_SEVERITY();
			DECLARE @conditions INT = ERROR_STATE();

			RAISERROR (@errors, @severity, @conditions);

	END CATCH
END
GO


BACKUP DATABASE PHARMACY TO DISK = 'D:\Projects\AppDev\Pharmacy_2\Database\MyBackup.bak' WITH DIFFERENTIAL;


SELECT * FROM ADD_USER;