void printOwing() 
{
	printBanner()

	outstanding = Getoutstanding()

	PrintDetail(outstanding)
	
}



double Getoutstanding() {
	double outstanding = 0.0;
	Enumeration e = _order.element();
	// outstanding 계산
	while(e.hasMoreElements())
	{
		Order each = (Order)e.nextElement();
		outstanding += each.getAmount();
	}
	return outstanding;
}

void PrintBanner() {
	// 배너(banner) 표시
	System.out.printIn(“*****************************”);
	System.out.printIn(“****** Customer Owes *******”);
	System.out.printIn(“*****************************”);
}

void PrintDetail(double outstanding) {
	// 상세 정보 표시
	System.out.printIn(“name:” + _name);
	System.out.printIn(“amount:” + outstanding);
}