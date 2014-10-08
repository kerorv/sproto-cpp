# AddressBook sproto

.Person {
    name 0 : string
    id 1 : integer
    email 2 : string

    .PhoneNumber {
        number 0 : string
        type 1 : integer
    }

    phones 3 : *PhoneNumber
}

.AddressBook {
    persons 0 : *Person
}

