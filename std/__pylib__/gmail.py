import smtplib

def send_gmail(sender, recipient, message, password):
    server = smtplib.SMTP('smtp.gmail.com', 587)
    server.starttls()
    server.login(sender, password)

    server.sendmail(sender, recipient, message)
    server.quit()

