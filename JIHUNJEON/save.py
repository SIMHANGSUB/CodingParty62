radius = input("반지름의 길이를 입력하세요 : ")

print("입력한 값은" + radius + " 입니다")
print(radius)

pi = 3.14159 # input is string, pi is float

# convert radius to float
radius = float(radius)

area = pi * radius * radius #area is float

print("원의 넓이는" + str(area) + "입니다")
